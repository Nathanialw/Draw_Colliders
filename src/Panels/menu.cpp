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

  enum class Color {
    BLACK,
    BLUE,
    WHITE,
    SIZE,
  };

  SDL_Color color[(int)Color::SIZE] = {
      {0, 0, 0, 255},
      {0, 0, 255, 255},
      {200, 200, 200, 255},
  };

  struct Button {
    std::vector<Button> subMenu;
    Action::Button action = Action::UNIMPLEMENTED;

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

  std::array<Button, SIZE> menu = {};

  void Add_Button(std::vector<Button> &btns, const std::string &text, const Action::Button &actionBtn) {
    Button btn;
    btn.text = text;
    btn.action = actionBtn;
    btns.emplace_back(btn);
  }

  void Init(App::App &app) {
    menu[FILE].text = "File";
    Add_Button(menu[FILE].subMenu, "New", Action::New_Project);
    Add_Button(menu[FILE].subMenu, "Open", Action::Open_Project);
    Add_Button(menu[FILE].subMenu, "Open Recent", Action::UNIMPLEMENTED);
    Add_Button(menu[FILE].subMenu, "Close", Action::Close_Project);
    Add_Button(menu[FILE].subMenu, "Publish", Action::Publish);
    Add_Button(menu[FILE].subMenu, "Publish As", Action::Publish_As);
    Add_Button(menu[FILE].subMenu, "Quit", Action::Quit_Application);

    menu[EDIT].text = "Edit";
    Add_Button(menu[EDIT].subMenu, "Delete Shape", Action::Delete_Selected_Shape);
    Add_Button(menu[EDIT].subMenu, "Delete Vertex", Action::Delete_Vertex_If_Polygon_Selected);
    Add_Button(menu[EDIT].subMenu, "New Point", Action::Create_Point);
    Add_Button(menu[EDIT].subMenu, "New Circle", Action::Create_Circle);
    Add_Button(menu[EDIT].subMenu, "New Line", Action::Create_Line);
    Add_Button(menu[EDIT].subMenu, "New AABB", Action::Create_Rect);
    Add_Button(menu[EDIT].subMenu, "New Polygon", Action::Create_Polygon);

    //to start open a new panel in the center of screen with 2 buttons that close the panel and an event loop to handle it
    menu[OPTIONS].text = "Options";
    Add_Button(menu[OPTIONS].subMenu, "Options", Action::UNIMPLEMENTED);
    Add_Button(menu[OPTIONS].subMenu, "Preferences", Action::UNIMPLEMENTED);
    Add_Button(menu[OPTIONS].subMenu, "Keybindings", Action::UNIMPLEMENTED);

    menu[VIEW].text = "View";
    Add_Button(menu[VIEW].subMenu, "Zoom++", Action::UNIMPLEMENTED);
    Add_Button(menu[VIEW].subMenu, "Zoom--", Action::UNIMPLEMENTED);
    Add_Button(menu[VIEW].subMenu, "Center", Action::UNIMPLEMENTED);

    //add current window name ie "New Document - Box2d_Colliders" and selection indicator checkmark
    menu[WINDOW].text = "Window";
    Add_Button(menu[WINDOW].subMenu, "current window", Action::UNIMPLEMENTED);

    menu[HELP].text = "Help";
    Add_Button(menu[HELP].subMenu, "Documentation", Action::UNIMPLEMENTED);


    float x = app.panel.menu.panel.x;
    float y = app.panel.menu.panel.y;
    float w = 50.0f;
    float h = app.panel.menu.panel.h;
    float spacing = 5.0f;

    for (auto & i : menu) {
      SDL_FRect dRect = {x + spacing, y, w, h};
      auto menuRect = Text::Render(app.context.renderer, app.context.font, i.text.c_str(), dRect.x, dRect.y);
      menuRect.w += (int)spacing * 3;
      menuRect.x -= (int)spacing;
      menuRect.h = h;

      float width = 0.0f;
      float offsetY = 0.0f;
      //get the longest word in submenu
      for (auto & j : i.subMenu) {
        auto rect = Text::Get_Rect_Size(app.context.renderer, app.context.font, j.text.c_str(), dRect.x, dRect.y, FC_ALIGN_LEFT);
        if (width < (float)rect.w)
          width = (float)rect.w;
      }

      //cache the rect
      i.panel = Rect_To_FRect(menuRect);
      i.subPanel = {
          (float)menuRect.x,
          dRect.y + i.panel.h,
          (float) width + (spacing * 2.0f),
          dRect.y + ((i.panel.h - (spacing / 2.0f)) * (float)i.subMenu.size())
      };

      app.menu.x = 0.0f;
      app.menu.y = 0.0f;
      app.menu.w += width;
      app.menu.h = (float) menuRect.h;
      x += (float)menuRect.w;
    }
  }

  bool Is_Open() {
    for (const auto &btn: menu)
      if (btn.render)
        return true;

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

    for (auto & i : menu) {
      SDL_FRect dRect = {x + spacing, y, w, h};
      //render to text, not every frame
      auto rect = Text::Get_Rect_Size(app.context.renderer, app.context.font, i.text.c_str(), dRect.x, dRect.y, FC_ALIGN_LEFT);
      float offsetY = (h - (float)rect.h) / 2.0f;

      rect.w += (int)spacing * 3;
      rect.x += (int)spacing / 2;
      rect.y += (int)offsetY;

      SDL_FRect fRect = {
          (float)x,
          (float)dRect.y,
          (float)rect.w,
          (float)h,
      };
      Graphics::Set_Render_Draw_Color(app.context.renderer, color[i.colorIndex]);
      SDL_RenderFillRectF(app.context.renderer, &fRect);
      Text::Render(app.context.renderer, app.context.font, i.text.c_str(), rect.x, rect.y);

      x += (float)rect.w;
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Subpanel(App::App &app) {
    for (auto & i : menu) {
      if (i.render) {
        float x = i.subPanel.x;
        float y = i.subPanel.y;
        float w = i.subPanel.w;
        float h = i.subPanel.h;
        float spacing = 5.0f;
        SDL_SetRenderDrawColor(app.context.renderer, 155, 25, 0, 255);

        //render to text, not every frame
        for (const auto &button : i.subMenu) {
          y += spacing;
          SDL_FRect dRect = {x + spacing, y, w, h};
          auto rect = Text::Render(app.context.renderer, app.context.font, button.text.c_str(), dRect.x, dRect.y);

          rect.w += (int)spacing * 2;
          rect.x -= (int)spacing;
          rect.w = (int)i.subPanel.w;

          SDL_FRect fRect = {
              (float)rect.x,
              (float)rect.y - (spacing /2.0f),
              (float)rect.w,
              (float)rect.h + spacing};

          if (Point_FRect_Intersect(Mouse::Cursor_Point(), fRect)) {
            SDL_SetRenderDrawColor(app.context.renderer, 55, 55, 55, 255);
            SDL_RenderFillRectF(app.context.renderer, &fRect);
            SDL_SetRenderDrawColor(app.context.renderer, 155, 25, 0, 255);
          }

          Text::Render(app.context.renderer, app.context.font, button.text.c_str(), dRect.x, dRect.y);
          y += (float)rect.h;
        }
        SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      }
    }
  }

  bool SubMenu_Select(App::App &app) {
    for (auto & i : menu) {
      if (i.render) {
        float x = i.subPanel.x;
        float y = i.subPanel.y;
        float w = i.subPanel.w;
        float h = i.subPanel.h;
        float spacing = 5.0f;

        //render to text, not every frame
        for (const auto &subMenu : i.subMenu) {
          y += spacing;
          SDL_FRect dRect = {x + spacing, y, w, h};
          auto rect = Text::Get_Rect_Size(app.context.renderer, app.context.font, subMenu.text.c_str(), dRect.x, dRect.y, FC_ALIGN_LEFT);
          auto fRect = Rect_To_FRect(rect);

          fRect.y -= (spacing /2.0f);
          fRect.w = i.subPanel.w;
          fRect.h += spacing;


          if (Point_FRect_Intersect(Mouse::Cursor_Point(), fRect)) {
            std::cout << subMenu.text << std::endl;
            subMenu.action(app);
            return true;
          }


          rect.w += (int)spacing * 2;
          rect.x -= (int)spacing;
          y += (float)rect.h;
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
        SDL_SetRenderDrawColor(app.context.renderer, 200, 200, 200, 100);
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