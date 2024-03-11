#pragma once

//
// Created by nathanial on 3/9/24.
//

#ifndef BOX2D_COLLIDERS_KEYBOARD_H
#define BOX2D_COLLIDERS_KEYBOARD_H

#endif //BOX2D_COLLIDERS_KEYBOARD_H

#include "../App/core.h"

namespace Keyboard {
#include "SDL2/SDL.h"

  bool Event(const SDL_Event &event, App::App &app);

}