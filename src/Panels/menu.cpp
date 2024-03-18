//
// Created by nathanial on 3/10/24.
//
#include "array"
#include "SDL2/SDL.h"
#include "menu.h"
#include "../Graphics/text.h"
#include "../Input/mouse.h"
#include "../Utils/utils.h"

namespace Menu {

  SDL_Color color[2] = {
      {0, 0, 0, 255},
      {0, 0, 255, 255},
  };


  struct Button {
    SDL_Texture* texture{};
    SDL_FRect rect{};
    SDL_FRect subPanel{};
    bool cache = false;
    bool render = false;
    int colorIndex = 0;
  };

  std::array<std::string, Graphics::numMenuButtons> text = {"File", "Edit", "Options", "View", "Window", "Help"};

  std::array<std::string, 7> file = {"New", "Open", " ", "Close", " ", " ", "Quit"};
  std::array<std::string, 7> edit = {"Delete Shape", "Delete Vertex", "New Polygon", "New AABB", "New Circle", "New Point", "New Line"};
  std::array<std::string, 7> options = {"", "", "", "", "", "", ""};
  std::array<std::string, 7> view = {"", "", "", "", "", "", ""};
  std::array<std::string, 7> window = {"", "", "", "", "", "", ""};
  std::array<std::string, 7> help = {"", "", "", "", "", "", ""};
  std::array<std::array<std::string, 7>, Graphics::numMenuButtons> subPanels = {
      file,
      edit,
      options,
      view,
      window,
      help,
  };
  std::array<Button, Graphics::numMenuButtons> menu = {};

  void Render_Panel(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, color[0]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.menu.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.menu.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Menu(App::App &app) {
    float x = app.panel.menu.panel.x;
    float y = app.panel.menu.panel.y;
    float w = 50.0f;
    float h = app.panel.menu.panel.h;
    float spacing = 5.0f;
    for (int i = 0; i < text.size(); ++i) {

      SDL_FRect dRect = {x + spacing, y, w, h};
      //render to text, not every frame
      auto rect = Text::Render(app.context.renderer, app.context.font, text[i].c_str(), dRect.x, dRect.y);
      rect.w += (int)spacing * 2;
      rect.x -= (int)spacing;
      Graphics::Set_Render_Draw_Color(app.context.renderer, color[menu[i].colorIndex]);
      SDL_RenderFillRect(app.context.renderer, &rect);
      Text::Render(app.context.renderer, app.context.font, text[i].c_str(), dRect.x, dRect.y);
      //cache the rect
      if (!menu[i].cache) {
        menu[i].rect = Rect_To_FRect(rect);
        menu[i].subPanel = {
            menu[i].rect.x,
            menu[i].rect.y + menu[i].rect.h,
            100.0f,
            300.0f
        };
        app.menu.x =  0.0f;
        app.menu.y =  0.0f;
        app.menu.w += (float)rect.w;
        app.menu.h =  (float)rect.h;
        menu[i].cache = true;
      }
      x += (float)rect.w;
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Subpanel(App::App &app) {
    for (int i = 0; i < menu.size(); ++i) {
      if (menu[i].render) {
        float x = menu[i].subPanel.x;
        float y = menu[i].subPanel.y;
        float w = 50.0f;
        float h = menu[i].subPanel.h;
        float spacing = 5.0f;
        SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
        //render to text, not every frame
        for (const auto &subpanel :subPanels[i]) {
          SDL_FRect dRect = {x + spacing, y, w, h};
          auto rect = Text::Render(app.context.renderer, app.context.font, subpanel.c_str(), dRect.x, dRect.y);
          rect.w += (int)spacing * 2;
          rect.x -= (int)spacing;
          SDL_RenderDrawRect(app.context.renderer, &rect);
          y += (float)rect.h;
        }
        SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      }
    }
  }

  void Render_Menu_Selected(App::App &app) {
    for (auto &button: menu) {
      if (button.render) {
        SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
        SDL_RenderFillRectF(app.context.renderer, &button.subPanel);
        SDL_SetRenderDrawColor(app.context.renderer, 20, 20, 255, 255);
        SDL_RenderDrawRectF(app.context.renderer, &button.subPanel);
      }
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

  }

  void Render(App::App &app) {
    Render_Panel(app);
    Render_Menu(app);
    Render_Menu_Selected(app);
    Render_Subpanel(app);
  }

  void Clear(App::App& app) {
    for (auto &button: menu) {
      button.render = false;
      app.menuOpen = false;
    }
  }

  bool Open(App::App& app) {
    auto cursor = Mouse::Cursor();

    for (auto &button: menu) {
      if (Rect_Intersect(button.rect, cursor)) {
        //toggle rendering panel at index
        button.render = !button.render;
        app.menuOpen = !app.menuOpen;
        return true;
      }
    }
    return false;
  }

  void Update(App::App& app) {
    auto cursor = Mouse::Cursor_Point();
    //if cursor is in the menu at all
    if (Point_FRect_Intersect(cursor, app.menu)) {
      for (auto &button: menu) {
        button.render = false;
        //point inside Rect instead

        if (Point_FRect_Intersect(cursor, button.rect)) {
          //highlight
          button.colorIndex = 1;
          //toggle rendering panel at index
          if (app.menuOpen) {
            button.render = true;
          }
        }
        else {
          button.colorIndex = 0;
        }
      }
    }
    else {
      for (auto &button: menu) {
        if (!button.render) {
          button.colorIndex = 0;
        }
      }
    }
  }
  //check with option is clicked
  //open panel with more options that can be clicked
  //the panel size will depend on the number of options
  //these option will be function pointers to actions ie open/close/quit
}