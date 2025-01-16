//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL.h"

#include "../App/core.h"
#include "mouse.h"
#include "actions.h"

#include "../Panels/menu.h"
#include "../Panels/top.h"
#include "../Panels/bottom.h"
#include "../Panels/center.h"

#include "../Panels/Center/center.h"
#include "../Panels/Center/left.h"
#include "../Panels/Center/right.h"

namespace Mouse {

  Uint32 doubleClickTimer = 0;
  bool doubleClick = false;

  bool Double_Click() {
    const Uint32 clickSpeed = 150;
    const Uint32 timer = (SDL_GetTicks() - doubleClickTimer);
    if (doubleClickTimer > 0) {
      if (timer <= clickSpeed) {
        doubleClickTimer = 0;
        doubleClick = true;
      }
      else {
        doubleClickTimer = 0;
      }
    }
    return true;
  }

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

  SDL_FPoint Cursor_Point() {
    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    return {(float)x, (float)y};
  }

  bool Down(const SDL_Event &event, App::App &app) {
    auto cursor = Cursor();
    if (event.type == SDL_MOUSEBUTTONDOWN) {
      Double_Click();

      if (app.filterImages) {
        app.filterImages = false;
        if (app.filterText.empty()) {
          app.filterText = app.filterTextDefault;
          app.interface.left.filteredIndexes.clear();
        }
      }

      if (event.button.button == SDL_BUTTON_LEFT) {
        if (Rect_Intersect(app.panel.menu.panel, cursor))
          if (Menu::Open(app)) {
            return true;
          }

        if (Menu::Is_Open()) {
          if (Menu::SubMenu_Select(app)) {
            Menu::Clear(app);
            return true;
            //action
          } else {
            Menu::Clear(app);
            return false;
            //close
          }
        }


        if (Rect_Intersect(app.panel.top.panel, cursor))
          return Top::Click_Menu_Button(app, Mouse::Cursor_Point());
        if (Rect_Intersect(app.panel.bottom, cursor))
          return Bottom::Bottom_Panel();

        if (Rect_Intersect(app.panel.center, cursor)) {
          if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor)) {
            if (Rect_Intersect(app.panel.mainPanel.right.scroll.bar, cursor)) {
              app.selected = SCROLLBAR_RIGHT;
              app.cachedScrollBarPosition = (Mouse::Cursor_Point().y - app.panel.mainPanel.right.scroll.bar.y);
              return true;
            }
            if (Rect_Intersect(app.panel.mainPanel.center.expanderRight, cursor)) {
              // while held save the offset of where the mouse was clicked and the current mouse position when released save the offset to the original value
              app.selected = EXPANDER_RIGHT;
              return true;
            }
            if (Rect_Intersect(app.panel.mainPanel.right.body, cursor)) {
              Center::Right::Select_Option(app);
              return true;
            }
            // modify settings
          }
          if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor)) {
            // select image
            if (Rect_Intersect(app.panel.mainPanel.left.body, cursor)) {
              return Center::Left::Set_Image(app);
            }
            // use search box
            if (Rect_Intersect(app.panel.mainPanel.left.search, cursor)) {
              return Center::Left::Filter_Images(app);
            }
            if (Rect_Intersect(app.panel.mainPanel.center.expanderLeft, cursor)) {
              // while held save the offset of where the mouse was clicked and the current mouse position when released save the offset to the original value
              app.selected = EXPANDER_LEFT;
              return true;
            }
            // use scroll
            if (Rect_Intersect(app.panel.mainPanel.left.scroll.bar, cursor)) {
              app.selected = SCROLLBAR_LEFT;
              app.cachedScrollBarPosition = (Mouse::Cursor_Point().y - app.panel.mainPanel.left.scroll.bar.y);
              return true;
            }
          };
          if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor)) {
            if (Rect_Intersect(app.panel.mainPanel.center.expanderLeft, cursor)) {
              // while held save the offset of where the mouse was clicked and the current mouse position when released save the offset to the original value
              app.selected = EXPANDER_LEFT;
              return true;
            }

            if (Rect_Intersect(app.panel.mainPanel.center.image, cursor)) {
              // check for vertex under mouse
              // select a vertex
              app.vertex = Shape::Get_Vertex(app.interface.center.texture, app.interface.center.shapes, app.vertexRadius, app.panel.mainPanel.center.image, cursor, app.offset, app.moveImage, app.zoomToMouse);
              bool vertexSelected = false;
              if (app.vertex.shape != Shape::SIZE) {
                vertexSelected = true;
                app.selectedVertex.shape = app.vertex.shape;
                app.selectedVertex.indexVertex = app.vertex.indexVertex;
                app.selectedVertex.indexPolygon = app.vertex.indexPolygon;
                if (app.vertex.shape == Shape::POINT) {
                  app.selectedShape.shape = app.vertex.shape;
                  app.selectedShape.indexPolygon = app.vertex.indexPolygon;
                }
                return Center::Center::Move_Vertex(app);
              }
              // select a shape
              // we want to grab the vertices too so we can move them when we move the shape
              app.vertex = Shape::Get_Shape(app.interface.center.texture, app.interface.center.shapes, app.vertexRadius, app.panel.mainPanel.center.image, app.offset, app.moveImage, app.zoomToMouse, cursor, app.selectedShape);
              if (app.vertex.shape == Shape::SIZE)
                return false;
              else {
                // move selected vertices
                if (doubleClick && app.selectedShape.shape == Shape::POLYGON) {
                  // create vertex at mouse
                  // need to calculate which edge the mouse is closest to and insert the vertex between those 2 points
                  Action::Add_Vertex(app);
                  doubleClick = false;
                  doubleClickTimer = 0;
                  return true;
                } else if (!vertexSelected) {
                  doubleClickTimer = SDL_GetTicks();
                  //                  app.selectedVertex.shape = app.vertex.shape;
                  //                  app.selectedVertex.indexVertex = app.vertex.indexVertex;
                  //                  app.selectedVertex.indexPolygon = app.vertex.indexPolygon;
                }
                app.selectedShape.shape = app.vertex.shape;
                app.selectedShape.indexPolygon = app.vertex.indexPolygon;
                return Center::Center::Move_Vertex(app);
              }
              // select from shape list

              // use scroll bar for shape list
              return true;
            }
            if (Rect_Intersect(app.panel.mainPanel.center.expanderRight, cursor)) {
              // while held save the offset of where the mouse was clicked and the current mouse position when released save the offset to the original value
              app.selected = EXPANDER_RIGHT;
              return true;
            }
            if (Rect_Intersect(app.panel.mainPanel.center.buttonBar.panel, cursor)) {
              for (int i = 0; i < app.panel.mainPanel.center.buttonBar.buttons.size(); ++i) {
                if (Rect_Intersect(app.panel.mainPanel.center.buttonBar.buttons[i].button, cursor)) {
                  Center::Center::Click_Button(app, i);
                };
              }
            }
            if (Rect_Intersect(app.panel.mainPanel.center.shapes.panel, cursor)) {
              if (Rect_Intersect(app.panel.mainPanel.center.shapes.expanderLeft, cursor)) {
                app.selected = EXPANDER_FIXTURES;
                return true;
              }
              if (Rect_Intersect(app.panel.mainPanel.center.shapes.body, cursor)) {
                auto vertex = Center::Center::Select_From_Shape_List_Names(app);
                app.selectedShape = {vertex.shape, vertex.indexPolygon};
                return true;
              }
              if (Rect_Intersect(app.panel.mainPanel.center.shapes.scroll.bar, cursor)) {
                app.selected = SCROLLBAR_FIXTURES;
                app.cachedScrollBarPosition = Mouse::Cursor_Point().y - app.panel.mainPanel.center.shapes.scroll.bar.y;
                return true;
              }
            }
          }
        }
        return true;
      }

      else if (event.button.button == SDL_BUTTON_RIGHT) {
        if (Rect_Intersect(app.panel.menu.panel, cursor)) {
          return true;
        }
        if (Rect_Intersect(app.panel.top.panel, cursor))
          return true;
        if (Rect_Intersect(app.panel.bottom, cursor))
          return true;
        if (Rect_Intersect(app.panel.center, cursor)) {
          if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor))
            return true;
          if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor))
            return true;
          if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor))
            return true;
        }
        return true;
      }

      else if (event.button.button == SDL_BUTTON_MIDDLE) {
        if (Rect_Intersect(app.panel.menu.panel, cursor)) {
          return true;
        }
        if (Rect_Intersect(app.panel.top.panel, cursor))
          return true;
        if (Rect_Intersect(app.panel.bottom, cursor))
          return true;
        if (Rect_Intersect(app.panel.center, cursor)) {
          if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor))
            return true;
          if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor))
            return true;
          if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor)) {
            if (Rect_Intersect(app.panel.mainPanel.center.image, cursor))
              return Center::Center::Move(app);
            if (Rect_Intersect(app.panel.mainPanel.center.expanderRight, cursor))
              return true;
            if (Rect_Intersect(app.panel.mainPanel.center.expanderLeft, cursor))
              return true;
            if (Rect_Intersect(app.panel.mainPanel.center.buttonBar.panel, cursor))
              return true;
          }
        }
        return true;
      }
      return true;
    }
    return false;
  }

  bool Up(const SDL_Event &event, App::App &app) {
    auto cursor = Cursor();

    if (event.type == SDL_MOUSEBUTTONUP) {
      if (event.button.button == SDL_BUTTON_LEFT) {
        Center::Center::Set_Expander(app);
        if (Rect_Intersect(app.panel.menu.panel, cursor)) {}
        else if (Rect_Intersect(app.panel.top.panel, cursor)) {}
        else if (Rect_Intersect(app.panel.bottom, cursor)) {}
        else if (Rect_Intersect(app.panel.center, cursor)) {
          if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor)) {}
          else if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor)) {}
          else if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor)) {
            if (Rect_Intersect(app.panel.mainPanel.center.image, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.expanderRight, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.expanderLeft, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.buttonBar.panel, cursor)) {
              for (const auto &button : app.panel.mainPanel.center.buttonBar.buttons) {
                if (Rect_Intersect(button.button, cursor)) {};
              }
            }
          }
        }
        //run regardless of where the mouse is when it is released, probably for all mouse button release actions
        Shape::Set_Vertex(app.interface.center.shapes, app.selectedShape, app.vertex, app.offset, app.interface.center.texture.scale, app.moveVertex, app.max);
        return true;
      }

      else if (event.button.button == SDL_BUTTON_RIGHT) {
        if (Rect_Intersect(app.panel.menu.panel, cursor)) {}
        else if (Rect_Intersect(app.panel.top.panel, cursor)) {}
        else if (Rect_Intersect(app.panel.bottom, cursor)){}
        else if (Rect_Intersect(app.panel.center, cursor)) {
          if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor)) {}
          else if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor)) {}
          else if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor)) {
            if (Rect_Intersect(app.panel.mainPanel.center.image, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.expanderRight, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.expanderLeft, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.buttonBar.panel, cursor)) {
              for (const auto &button : app.panel.mainPanel.center.buttonBar.buttons) {
                if (Rect_Intersect(button.button, cursor)) {};
              }
            }
          }
        }
        return true;
      }

      else if (event.button.button == SDL_BUTTON_MIDDLE) {
        if (Rect_Intersect(app.panel.menu.panel, cursor)) {}
        else if (Rect_Intersect(app.panel.top.panel, cursor)) {}
        else if (Rect_Intersect(app.panel.bottom, cursor)){}
        else if (Rect_Intersect(app.panel.center, cursor)) {
          if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor)) {}
          else if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor)) {}
          else if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor)) {
            if (Rect_Intersect(app.panel.mainPanel.center.image, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.expanderRight, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.expanderLeft, cursor)) {}
            else if (Rect_Intersect(app.panel.mainPanel.center.buttonBar.panel, cursor)) {
              for (const auto &button : app.panel.mainPanel.center.buttonBar.buttons) {
                if (Rect_Intersect(button.button, cursor)) {};
              }
            }
          }
        }

        //run regardless of where the mouse is when it is released, probably for all mouse button release actions
        Center::Center::Set(app.max, app.offset, app.interface.center.texture.offset, app.moveImage);
        return true;
      }
      return true;
    }

    if (event.type == SDL_MOUSEWHEEL) {
      if (Rect_Intersect(app.panel.mainPanel.center.panel, cursor)) {
        if (Rect_Intersect(app.panel.mainPanel.center.image, cursor))
          return Center::Center::Set_Scale(app, event.wheel.y);
        if (Rect_Intersect(app.panel.mainPanel.center.shapes.body, cursor))
          return Center::Center::Scroll(app, event.wheel.y);
      }
      if (Rect_Intersect(app.panel.mainPanel.left.panel, cursor))
        return Center::Left::Scroll(app, event.wheel.y);
      if (Rect_Intersect(app.panel.mainPanel.right.panel, cursor))
        return Center::Right::Scroll(app, event.wheel.y);
      return true;
    }
    return false;
  };

  bool Event(const SDL_Event &event, App::App &app) {
    if (Up(event, app))
      return true;
    if (Down(event, app))
      return true;
    return false;
  }

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
