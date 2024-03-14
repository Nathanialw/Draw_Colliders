//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL.h"
#include "string"
#include "iostream"

#include "../App/core.h"
#include "mouse.h"
#include "../Utils/utils.h"

#include "../Panels/menu.h"
#include "../Panels/top.h"
#include "../Panels/bottom.h"
#include "../Panels/center.h"

#include "../Panels/Center/center.h"
#include "../Panels/Center/left.h"
#include "../Panels/Center/right.h"

namespace Mouse {

  bool Set_Cursor(App::App &app, const SDL_SystemCursor &cursor) {
    SDL_FreeCursor(app.cursor);
    app.cursor = SDL_CreateSystemCursor(cursor);
    SDL_SetCursor(app.cursor);
    return true;
  }

  SDL_FRect Cursor() {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    return {(float)x - 5.0f, (float)y - 5.0f, 10.0f, 10.0f};
  }

  bool Event(const SDL_Event &event, App::App &app) {
    //check which panel the mouse is inside of
    auto cursor = Cursor();
    if (event.type == SDL_MOUSEBUTTONDOWN) {

      if (app.filterImages){
        app.filterImages = false;
        if (app.filterText.empty())
          app.filterText = app.filterTextDefault;
      }

      if (event.button.button == SDL_BUTTON_LEFT) {
        if (SDL_HasIntersectionF(&app.panel.menu.panel, &cursor))
          return Menu::Top_Menu();
        if (SDL_HasIntersectionF(&app.panel.top.panel, &cursor))
          return Top::Top_Panel();
        if (SDL_HasIntersectionF(&app.panel.bottom, &cursor))
          return Bottom::Bottom_Panel();

        if (SDL_HasIntersectionF(&app.panel.center, &cursor)) {
          if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor)){
            //modify settings
            return true;
          }
          if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor)) {
            // select image
            if (SDL_HasIntersectionF(&app.panel.mainPanel.left.body, &cursor)) {
              return Center::Left::Set_Image(app);
            }
              //use search box
            if (SDL_HasIntersectionF(&app.panel.mainPanel.left.search, &cursor)) {
              return Center::Left::Filter_Images(app);
            }
             //use scroll
            if (SDL_HasIntersectionF(&app.panel.mainPanel.left.scroll.bar, &cursor)) {
              return Center::Left::Scroll(app);
            }
          };
          if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor))
            {
              //check for vertex under mouse
              //select a vertex
              app.vertex = App::Get_Vertex(app, cursor);
              if (app.vertex.shape == Graphics::SIZE)
              //select a shape
                app.vertex = App::Get_Shape(app, cursor);
              if (app.vertex.shape == Graphics::SIZE)
                return false;
              else {
              //move selected vertexes
                return Center::Center::Move_Vertex(app);
              }
              //select from shape list

              //use scroll bar for shape list
              return true;
            }
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor)) {
              //while held save the offset of where the mouse was clicked and the current mouse position
              //when released save the offset to the original value
              return true;
            }
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor)) {
              //while held save the offset of where the mouse was clicked and the current mouse position
              //when released save the offset to the original value
              return true;
            }
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.buttonBar.panel, &cursor)) {
              for (int i = 0; i < app.panel.mainPanel.center.buttonBar.buttons.size(); ++i) {
                if (SDL_HasIntersectionF(&app.panel.mainPanel.center.buttonBar.buttons[i], &cursor)) {
                  Center::Center::Click_Button(app, i);
                };
              }
            }
          }
        }
        return true;
      }
      else if (event.button.button == SDL_BUTTON_RIGHT) {
        if (SDL_HasIntersectionF(&app.panel.menu.panel, &cursor)) {
          return true;
        }
        if (SDL_HasIntersectionF(&app.panel.top.panel, &cursor))
          return true;
        if (SDL_HasIntersectionF(&app.panel.bottom, &cursor))
          return true;
        if (SDL_HasIntersectionF(&app.panel.center, &cursor)) {
          if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor))
            return true;
          if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor))
            return true;
          if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor))
            return true;
        }
        return true;
      }
      else if (event.button.button == SDL_BUTTON_MIDDLE) {
        if (SDL_HasIntersectionF(&app.panel.menu.panel, &cursor)) {
          return true;
        }
        if (SDL_HasIntersectionF(&app.panel.top.panel, &cursor))
          return true;
        if (SDL_HasIntersectionF(&app.panel.bottom, &cursor))
          return true;
        if (SDL_HasIntersectionF(&app.panel.center, &cursor)) {
          if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor))
            return true;
          if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor))
            return true;
          if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor))
              return Center::Center::Move(app);
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor))
              return true;
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor))
              return true;
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.buttonBar.panel, &cursor))
              return true;
          }
        }
        return true;
      }
      return true;
    }

    if (event.type == SDL_MOUSEBUTTONUP) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        if (SDL_HasIntersectionF(&app.panel.menu.panel, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.top.panel, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.bottom, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.center, &cursor)) {
          if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor)) {}
          else if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor)) {}
          else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.buttonBar.panel, &cursor)) {
              for (const auto &button : app.panel.mainPanel.center.buttonBar.buttons) {
                if (SDL_HasIntersectionF(&button, &cursor)) {};
              }
            }
          }
        }
        //run regardless of where the mouse is when it is released, probably for all mouse button release actions
        if (app.vertex.shape != Graphics::SIZE) {
          Center::Center::Set_Vertex(app);
          app.vertex = {Graphics::SIZE, 0, 0};
        }
        return true;
      }

      else if (event.button.button == SDL_BUTTON_RIGHT) {
        if (SDL_HasIntersectionF(&app.panel.menu.panel, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.top.panel, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.bottom, &cursor)){}
        else if (SDL_HasIntersectionF(&app.panel.center, &cursor)) {
          if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor)) {}
          else if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor)) {}
          else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.buttonBar.panel, &cursor)) {
              for (const auto &button : app.panel.mainPanel.center.buttonBar.buttons) {
                if (SDL_HasIntersectionF(&button, &cursor)) {};
              }
            }
          }
        }
        return true;
      }

      else if (event.button.button == SDL_BUTTON_MIDDLE) {
        if (SDL_HasIntersectionF(&app.panel.menu.panel, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.top.panel, &cursor)) {}
        else if (SDL_HasIntersectionF(&app.panel.bottom, &cursor)){}
        else if (SDL_HasIntersectionF(&app.panel.center, &cursor)) {
          if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor)) {}
          else if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor)) {}
          else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
            if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor)) {}
            else if (SDL_HasIntersectionF(&app.panel.mainPanel.center.buttonBar.panel, &cursor)) {
              for (const auto &button : app.panel.mainPanel.center.buttonBar.buttons) {
                if (SDL_HasIntersectionF(&button, &cursor)) {};
              }
            }
          }
        }

        //run regardless of where the mouse is when it is released, probably for all mouse button release actions
        Center::Center::Set(app, app.interface.center.texture.offset);
        return true;
      }
      return true;
    }

    if (event.type == SDL_MOUSEWHEEL) {
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor))
        return Center::Center::Set_Scale(app, event.wheel.y);
      if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor))
        return Center::Left::Scroll(app);
      if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor))
        return Center::Right::Scroll(app);
      return true;
    }
    return false;
  };

  bool Motion(const SDL_Event &event, App::App &app, int &x) {
    if (event.type == SDL_MOUSEMOTION) {
      x++;
      app.wait = false;
      if (x > app.max)
        return true;
    }
    return false;
  }

  bool Update_Cursor(App::App &app) {
    SDL_SystemCursor desiredCursorId = Center::Mouse(app);

    if (desiredCursorId != app.currentCursorId) {
      app.currentCursorId = desiredCursorId;
      Mouse::Set_Cursor(app, desiredCursorId);
      return true;
    }
    return false;
  }

  void Render(App::App &app) {
    auto cursor = Cursor();
    SDL_SetRenderDrawColor(app.context.renderer, 205, 205, 205, 255);
    SDL_RenderFillRectF(app.context.renderer, &cursor);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }
}
