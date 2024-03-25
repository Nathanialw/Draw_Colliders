//
// Created by nathanial on 3/10/24.
//
#include "Center/left.h"
#include "Center/center.h"
#include "Center/right.h"
#include "center.h"
#include "../Input/mouse.h"
#include "../App/core.h"
#include "menu.h"

namespace Center {
  SDL_SystemCursor Mouse(App::App &app) {
    auto cursor = Mouse::Cursor();

    if (Menu::Is_Open())
      return SDL_SYSTEM_CURSOR_ARROW;

    if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor)) {
      if (SDL_HasIntersectionF(&app.panel.mainPanel.left.search, &cursor)) {
        return SDL_SYSTEM_CURSOR_IBEAM;
      }
      if (SDL_HasIntersectionF(&app.panel.mainPanel.left.body, &cursor)) {
        if (Left::Hover_Image(app)) {
          return SDL_SYSTEM_CURSOR_HAND;
        }
      }
    }

    if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor)) {
        return SDL_SYSTEM_CURSOR_SIZEWE;
      }
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.shapes.expanderLeft, &cursor)) {
        return SDL_SYSTEM_CURSOR_SIZEWE;
      }
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor)) {
        return SDL_SYSTEM_CURSOR_SIZEWE;
      }
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor)) {
        if (app.moveImage) return SDL_SYSTEM_CURSOR_HAND;
        else return SDL_SYSTEM_CURSOR_CROSSHAIR;
      }
    }

    if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor)){

    }

    return SDL_SYSTEM_CURSOR_ARROW;
  };



}