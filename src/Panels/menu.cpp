//
// Created by nathanial on 3/10/24.
//
#include "array"
#include "SDL2/SDL.h"
#include "menu.h"
#include "../Graphics/text.h"
#include "../Input/mouse.h"
#include "../Input/actions.h"
#include "iostream"

namespace Menu {

  SDL_Color color[2] = {
      {0, 0, 0, 255},
      {0, 0, 255, 255},
  };

  struct Button {
    std::vector<Button> subMenu;
    Action::Button action = Action::Unused;

    SDL_FRect panel{};
    SDL_FRect subPanel{};
    int8_t colorIndex = 0;
    std::string text;
    bool render = false;
  };

  enum Menu_Labels {
    FILE,
    EDIT,
    OPTIONS,
    VIEW,
    WINDOW,
    HELP,
    SIZE
  };

  std::vector<Button> menu = {};

  void Init(App::App &app) {
    menu.resize(6);
    menu[FILE].text = "File";
    menu[FILE].subMenu.resize(7);
    menu[FILE].subMenu[0].text = "New";
    menu[FILE].subMenu[0].action;
    menu[FILE].subMenu[1].text = "Open";
    menu[FILE].subMenu[1].action;
    menu[FILE].subMenu[2].text = "Open Recent";
    menu[FILE].subMenu[2].action;
    menu[FILE].subMenu[3].text = "Close";
    menu[FILE].subMenu[3].action;
    menu[FILE].subMenu[4].text = " ";
    menu[FILE].subMenu[4].action;
    menu[FILE].subMenu[5].text = " ";
    menu[FILE].subMenu[5].action;
    menu[FILE].subMenu[6].text = "Quit";
    menu[FILE].subMenu[6].action;

    menu[EDIT].text = "Edit";
    menu[EDIT].subMenu.resize(8);
    menu[EDIT].subMenu[0].text = "Delete Shape";
    menu[EDIT].subMenu[0].action = Action::Delete_Selected_Shape;
    menu[EDIT].subMenu[1].text = "Delete Vertex";
    menu[EDIT].subMenu[1].action = Action::Delete_Vertex_If_Polygon_Selected;
    menu[EDIT].subMenu[2].text = " ";
    menu[EDIT].subMenu[3].text = "New Point";
    menu[EDIT].subMenu[3].action = Action::Create_Point;
    menu[EDIT].subMenu[4].text = "New Circle";
    menu[EDIT].subMenu[4].action = Action::Create_Circle;
    menu[EDIT].subMenu[5].text = "New Line";
    menu[EDIT].subMenu[5].action = Action::Create_Line;
    menu[EDIT].subMenu[6].text = "New AABB";
    menu[EDIT].subMenu[6].action = Action::Create_Rect;
    menu[EDIT].subMenu[7].text = "New Polygon";
    menu[EDIT].subMenu[7].action = Action::Create_Polygon;

    menu[OPTIONS].text = "Options";
    menu[OPTIONS].subMenu.resize(2);
    menu[OPTIONS].subMenu[0].text = "options";
    menu[OPTIONS].subMenu[0].action;
    menu[OPTIONS].subMenu[1].text = "preferences";
    menu[OPTIONS].subMenu[1].action;

    menu[VIEW].text = "View";
    menu[VIEW].subMenu.resize(3);
    menu[VIEW].subMenu[0].text = "Zoom++";
    menu[VIEW].subMenu[0].action;
    menu[VIEW].subMenu[1].text = "Zoom--";
    menu[VIEW].subMenu[1].action;
    menu[VIEW].subMenu[2].text = "Center";
    menu[VIEW].subMenu[2].action;

    //add current window name ie "New Document - Box2d_Colliders" and selection indicator checkmark
    menu[WINDOW].text = "Window";
    menu[WINDOW].action;

    menu[HELP].text = "Help";
    menu[HELP].subMenu.resize(1);
    menu[HELP].subMenu[0].text = "Documentation";
    menu[HELP].subMenu[0].action;


    float x = app.panel.menu.panel.x;
    float y = app.panel.menu.panel.y;
    float w = 50.0f;
    float h = app.panel.menu.panel.h;
    float spacing = 5.0f;

    for (int i = 0; i < menu.size(); ++i) {
      SDL_FRect dRect = {x + spacing, y, w, h};
      auto menuRect = Text::Render(app.context.renderer, app.context.font, menu[i].text.c_str(), dRect.x, dRect.y);
      menuRect.w += (int)spacing * 2;
      menuRect.x -= (int)spacing;

      float width = 0.0f;

      //get longest word in submenu
      for (int j = 0; j < menu[i].subMenu.size(); ++j) {
        auto rect = Text::Render(app.context.renderer, app.context.font, menu[i].subMenu[j].text.c_str(), dRect.x, dRect.y);
        if (width < rect.w)
          width = rect.w;
      }

      //cache the rect
      menu[i].panel = Rect_To_FRect(menuRect);
      menu[i].subPanel = {
          (float)menuRect.x,
          dRect.y + menu[i].panel.h,
          (float) width + (spacing * 2.0f),
          dRect.y + ((menu[i].panel.h + spacing) * menu[i].subMenu.size())
      };
      app.menu.x = 0.0f;
      app.menu.y = 0.0f;
      app.menu.w += width;
      app.menu.h = (float) menuRect.h;
      x += menuRect.w;
    }
  }

  bool Is_Open() {
    for (const auto &btn: menu) {
      if (btn.render)
        return true;
    }
    return false;
  }

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

    float subMenuWidth = 0.0f;
    for (int i = 0; i < menu.size(); ++i) {
      SDL_FRect dRect = {x + spacing, y, w, h};
      //render to text, not every frame
      auto rect = Text::Render(app.context.renderer, app.context.font, menu[i].text.c_str(), dRect.x, dRect.y);
      rect.w += (int)spacing * 2;
      rect.x -= (int)spacing;
      Graphics::Set_Render_Draw_Color(app.context.renderer, color[menu[i].colorIndex]);
      SDL_RenderFillRect(app.context.renderer, &rect);
      Text::Render(app.context.renderer, app.context.font, menu[i].text.c_str(), dRect.x, dRect.y);

      x += (float)rect.w;
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Subpanel(App::App &app) {
    for (int i = 0; i < menu.size(); ++i) {
      if (menu[i].render) {
        float x = menu[i].subPanel.x;
        float y = menu[i].subPanel.y;
        float w = menu[i].subPanel.w;
        float h = menu[i].subPanel.h;
        float spacing = 5.0f;
        SDL_SetRenderDrawColor(app.context.renderer, 155, 25, 0, 255);

        //render to text, not every frame
        float subMenuH = 0.0f;
        for (const auto &subpanel : menu[i].subMenu) {
          SDL_FRect dRect = {x + spacing, y, w, h};
          auto rect = Text::Render(app.context.renderer, app.context.font, subpanel.text.c_str(), dRect.x, dRect.y);
          if (rect.h > 0.0f)
            subMenuH = rect.h;

          rect.w += (int)spacing * 2;
          rect.x -= (int)spacing;
          rect.w = menu[i].subPanel.w;
          SDL_RenderDrawRect(app.context.renderer, &rect);
          y += subMenuH + spacing;
        }
        SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      }
    }
  }

  bool SubMenu_Select(App::App &app) {
    for (int i = 0; i < menu.size(); ++i) {
      if (menu[i].render) {
        float x = menu[i].subPanel.x;
        float y = menu[i].subPanel.y;
        float w = menu[i].subPanel.w;
        float h = menu[i].subPanel.h;
        float spacing = 5.0f;

        //render to text, not every frame
        float subMenuH = 0.0f;
        for (const auto &subpanel : menu[i].subMenu) {
          SDL_FRect dRect = {x + spacing, y, w, h};
          auto rect = Text::Render(app.context.renderer, app.context.font, subpanel.text.c_str(), dRect.x, dRect.y);
          auto fRect = Rect_To_FRect(rect);
          fRect.w = menu[i].subPanel.w;
          if (Point_FRect_Intersect(Mouse::Cursor_Point(), fRect)) {
            std::cout << subpanel.text << std::endl;
            subpanel.action(app);
            return true;
          }

          if (rect.h > 0.0f)
            subMenuH = rect.h;

          rect.w += (int)spacing * 2;
          rect.x -= (int)spacing;
          y += subMenuH + spacing;
        }
      }
    }
    return false;
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
      if (Rect_Intersect(button.panel, cursor)) {
        //toggle rendering panel at index
        button.render = !button.render;
        app.menuOpen = !app.menuOpen;
        return true;
      }
    }
    return false;
  }

  void Update(App::App& app) {
    app.zoomToMouse = false;
    auto cursor = Mouse::Cursor_Point();
    //if cursor is in the menu at all
    if (Point_FRect_Intersect(cursor, app.menu)) {
      for (auto &button: menu) {
        button.render = false;
        //point inside Rect instead

        if (Point_FRect_Intersect(cursor, button.panel)) {
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
//
//    for (int i = 0; i < menu.size(); ++i) {
//      if (menu[i].render) {
//        float x = menu[i].subPanel.x;
//        float y = menu[i].subPanel.y;
//        float w = menu[i].subPanel.w;
//        float h = menu[i].subPanel.h;
//        float spacing = 5.0f;
//
//        //render to text, not every frame
//        float subMenuH = 0.0f;
//        for (const auto &subpanel :subPanels[i]) {
//          SDL_FRect dRect = {x + spacing, y, w, h};
//          auto rect = Text::Render(app.context.renderer, app.context.font, subpanel.c_str(), dRect.x, dRect.y);
//          auto fRect = Rect_To_FRect(rect);
//          fRect.w = menu[i].subPanel.w;
//          if (Point_FRect_Intersect(Mouse::Cursor_Point(), fRect)) {
//            std::cout << subpanel << std::endl;
//          }
//
//          if (rect.h > 0.0f)
//            subMenuH = rect.h;
//
//          rect.w += (int)spacing * 2;
//          rect.x -= (int)spacing;
//          y += subMenuH;
//        }
//      }
//    }
  }
  //check with option is clicked
  //open panel with more options that can be clicked
  //the panel size will depend on the number of options
  //these option will be function pointers to actions ie open/close/quit
}