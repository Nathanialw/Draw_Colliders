#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_POINT_H
#define BOX2D_COLLIDERS_POINT_H

#endif //BOX2D_COLLIDERS_POINT_H

#include "../../Graphics/graphics.h"


namespace Point {
  struct Point {
    SDL_FPoint vertex{};
    bool moving = false;
  };

  Point Create(const Graphics::Center_Panel &panel);
}