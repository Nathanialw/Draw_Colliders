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

  struct Button {
    std::vector<Button> subMenu;
    Action::Button action = Action::UNIMPLEMENTED;

    SDL_FRect panel{};
    SDL_FRect subPanel{};
    Graphics::Color colorIndex = Graphics::BACKGROUND;
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

  Button Add_Button(const std::string &text, const Action::Button &actionBtn) {
    Button btn;
    btn.text = text;
    btn.action = actionBtn;
    return btn;
  }

  void Init(App::App &app) {
    menu[FILE].text = "File";
    menu[FILE].subMenu.emplace_back(Add_Button("New", Action::New_Project));
    menu[FILE].subMenu.emplace_back(Add_Button("Open", Action::Open_Project));
    menu[FILE].subMenu.emplace_back(Add_Button("Open Recent", Action::UNIMPLEMENTED));
    menu[FILE].subMenu.emplace_back(Add_Button("Close", Action::Close_Project));
    menu[FILE].subMenu.emplace_back(Add_Button("Publish", Action::Publish));
    menu[FILE].subMenu.emplace_back(Add_Button("Publish As", Action::Publish_As));
    menu[FILE].subMenu.emplace_back(Add_Button("Quit", Action::Quit_Application));

    menu[EDIT].text = "Edit";
    menu[EDIT].subMenu.emplace_back(Add_Button("Delete Shape", Action::Delete_Selected_Shape));
    menu[EDIT].subMenu.emplace_back(Add_Button("Delete Vertex", Action::Delete_Vertex_If_Polygon_Selected));
    menu[EDIT].subMenu.emplace_back(Add_Button("New Point", Action::Create_Point));
    menu[EDIT].subMenu.emplace_back(Add_Button("New Circle", Action::Create_Circle));
    menu[EDIT].subMenu.emplace_back(Add_Button("New Line", Action::Create_Line));
    menu[EDIT].subMenu.emplace_back(Add_Button("New AABB", Action::Create_Rect));
    menu[EDIT].subMenu.emplace_back(Add_Button("New Polygon", Action::Create_Polygon));

    //to start open a new panel in the center of screen with 2 buttons that close the panel and an event loop to handle it
    menu[OPTIONS].text = "Options";
    menu[OPTIONS].subMenu.emplace_back(Add_Button("Options", Action::UNIMPLEMENTED));
    menu[OPTIONS].subMenu.emplace_back(Add_Button("Preferences", Action::UNIMPLEMENTED));
    menu[OPTIONS].subMenu.emplace_back(Add_Button("Keybindings", Action::UNIMPLEMENTED));

    menu[VIEW].text = "View";
    menu[VIEW].subMenu.emplace_back(Add_Button("Zoom++", Action::UNIMPLEMENTED));
    menu[VIEW].subMenu.emplace_back(Add_Button("Zoom--", Action::UNIMPLEMENTED));
    menu[VIEW].subMenu.emplace_back(Add_Button("Center", Action::UNIMPLEMENTED));

    //add current window name ie "New Document - Box2d_Colliders" and selection indicator checkmark
    menu[WINDOW].text = "Window";
    menu[WINDOW].subMenu.emplace_back(Add_Button("current window", Action::UNIMPLEMENTED));

    menu[HELP].text = "Help";
    menu[HELP].subMenu.emplace_back(Add_Button("Documentation", Action::UNIMPLEMENTED));


    float x = app.panel.menu.panel.x;
    float y = app.panel.menu.panel.y;
    float w = 50.0f;
    float h = app.panel.menu.panel.h;
    float spacing = 5.0f;

    for (auto & i : menu) {
      SDL_FRect dRect = {x + spacing, y, w, h};
      auto menuRect = Text::Get_Rect_Size(app.context.renderer, app.context.font, i.text.c_str(), dRect.x, dRect.y, FC_ALIGN_LEFT);
      auto panel = Rect_To_FRect(menuRect);
      panel.w += spacing * 3.0f;
      panel.x -= spacing;
      panel.h = h;

      float width = 0.0f;
      float offsetY = 0.0f;
      //get the longest word in submenu
      for (auto & j : i.subMenu) {
        auto rect = Text::Get_Rect_Size(app.context.renderer, app.context.font, j.text.c_str(), dRect.x, dRect.y, FC_ALIGN_LEFT);
        if (width < (float)rect.w)
          width = (float)rect.w;
      }

      //cache the rect
      i.panel = panel;
      i.subPanel = {
          panel.x,
          dRect.y + i.panel.h,
          (float) width + (spacing * 2.0f),
          dRect.y + ((i.panel.h - (spacing / 2.0f)) * (float)i.subMenu.size())
      };

      app.menu.x = 0.0f;
      app.menu.y = 0.0f;
      app.menu.w += width;
      app.menu.h = panel.h;
      x += (float)panel.w;
    }
  }

  bool Is_Open() {
    for (const auto &btn: menu)
      if (btn.render)
        return true;

    return false;
  }

  void Render_Panel(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.menu.panel);
//    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
//    SDL_RenderDrawRectF(app.context.renderer, &app.panel.menu.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
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
      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[i.colorIndex]);
      SDL_RenderFillRectF(app.context.renderer, &fRect);
      Text::Render(app.context.renderer, app.context.font, i.text.c_str(), rect.x, rect.y);

      x += (float)rect.w;
    }
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
  }

  void Render_Subpanel(App::App &app) {
    for (auto & i : menu) {
      if (i.render) {
        float x = i.subPanel.x;
        float y = i.subPanel.y;
        float w = i.subPanel.w;
        float h = i.subPanel.h;
        float spacing = 5.0f;
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);

        //render to text, not every frame
        for (const auto &button : i.subMenu) {
          y += spacing;
          SDL_FRect dRect = {x + spacing, y, w, h};
          auto rect = Text::Get_Rect_Size(app.context.renderer, app.context.font, i.text.c_str(), dRect.x, dRect.y, FC_ALIGN_LEFT);

          rect.w += (int)spacing * 2;
          rect.x -= (int)spacing;
          rect.w = (int)i.subPanel.w;

          SDL_FRect fRect = {
              (float)rect.x,
              (float)rect.y - (spacing /2.0f),
              (float)rect.w,
              (float)rect.h + spacing};

          if (Point_FRect_Intersect(Mouse::Cursor_Point(), fRect)) {
            Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
            SDL_RenderFillRectF(app.context.renderer, &fRect);
            Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
          }

          Text::Render(app.context.renderer, app.context.font, button.text.c_str(), dRect.x, dRect.y);
          y += (float)rect.h;
        }
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
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
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
        SDL_RenderFillRectF(app.context.renderer, &button.subPanel);
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
        SDL_RenderDrawRectF(app.context.renderer, &button.subPanel);
      }
    }
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
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
          button.colorIndex = Graphics::SELECTED;
          //toggle rendering panel at index
          if (app.menuOpen) {
            button.render = true;
          }
        }
        else {
          button.colorIndex = Graphics::BACKGROUND;
        }
      }
    }
    else {
      for (auto &button: menu) {
        if (!button.render) {
          button.colorIndex = Graphics::BACKGROUND;
        }
      }
    }
  }
}