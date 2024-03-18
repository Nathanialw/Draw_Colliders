#pragma once

//
// Created by nathanial on 3/9/24.
//

#ifndef BOX2D_COLLIDERS_MOUSE_H
#define BOX2D_COLLIDERS_MOUSE_H

#endif //BOX2D_COLLIDERS_MOUSE_H


#include "../App/core.h"
#include "SDL2/SDL.h"

namespace Mouse {
  SDL_FRect Cursor();
  SDL_FPoint Cursor_Point();
  bool Update_Cursor(App::App &app);
  void Render(App::App &app);
  bool Set_Cursor(App::App &app, const SDL_SystemCursor &cursor);
  bool Event(const SDL_Event &event, App::App &app);
  bool Motion(const SDL_Event &event, App::App &app, int &x);
}
