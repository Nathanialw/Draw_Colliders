//
// Created by nathanial on 3/10/24.
//
#include "array"
#include "SDL2/SDL.h"
#include "menu.h"
#include "../Graphics/text.h"
#include "../Input/mouse.h"

namespace Menu {

  struct Button {
    SDL_Texture* texture{};
    SDL_FRect rect{};
    SDL_FRect subPanel{};
    bool cache = false;
    bool render = false;
  };

  std::array<std::string, Graphics::numMenuButtons> text = {"File", "Edit", "Options", "View", "Window", "Help"};

  std::array<std::string, 7> file = {"New", "Open", "", "Close", "", "", ""};
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
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
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
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    for (int i = 0; i < text.size(); ++i) {

      SDL_FRect dRect = {x + spacing, y, w, h};
      //render to text, not every frame
      auto rect = Text::Render(app.context.renderer, app.context.font, text[i].c_str(), dRect.x, dRect.y);
      rect.w += (int)spacing * 2;
      rect.x -= (int)spacing;
      SDL_RenderDrawRect(app.context.renderer, &rect);
      //cache the rect
      if (!menu[i].cache) {
        menu[i].rect = Rect_To_FRect(rect);
        menu[i].subPanel = {
            menu[i].rect.x,
            menu[i].rect.y + menu[i].rect.h,
            100.0f,
            300.0f
        };
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

  void Clear() {
    for (auto &button: menu) {
      button.render = false;
    }
  }

  bool Open(App::App& app) {
    auto cursor = Mouse::Cursor();

    for (auto &button: menu) {
      if (Rect_Intersect(button.rect, cursor)) {
        //toggle rendering panel at index
        button.render = true;
        return true;
      }
    }
    return false;
  }
  //check with option is clicked
  //open panel with more options that can be clicked
  //the panel size will depend on the number of options
  //these option will be function pointers to actions ie open/close/quit
}