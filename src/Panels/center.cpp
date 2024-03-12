//
// Created by nathanial on 3/10/24.
//
#include "Center/left.h"
#include "Center/center.h"
#include "Center/right.h"
#include "center.h"
#include "../Input/mouse.h"
#include "../App/core.h"

namespace Center {
  SDL_SystemCursor Mouse(App::App &app) {
    auto cursor = Mouse::Cursor();

    if (SDL_HasIntersectionF(&app.panel.mainPanel.left.panel, &cursor)) {
      if (Left::Select_Image(app).texture) {
        return SDL_SYSTEM_CURSOR_HAND;
      }
    }

    if (SDL_HasIntersectionF(&app.panel.mainPanel.center.panel, &cursor)) {
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.image, &cursor)) {
        if (app.moveImage) return SDL_SYSTEM_CURSOR_HAND;
        else return SDL_SYSTEM_CURSOR_CROSSHAIR;
      }
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderRight, &cursor)) {
        return SDL_SYSTEM_CURSOR_SIZEWE;
      }
      if (SDL_HasIntersectionF(&app.panel.mainPanel.center.expanderLeft, &cursor)) {
        return SDL_SYSTEM_CURSOR_SIZEWE;
      }
    }

    if (SDL_HasIntersectionF(&app.panel.mainPanel.right.panel, &cursor)){

    }

    return SDL_SYSTEM_CURSOR_ARROW;
  };



}