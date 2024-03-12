#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_AABB_H
#define BOX2D_COLLIDERS_AABB_H

#endif //BOX2D_COLLIDERS_AABB_H

#include "../../Graphics/graphics.h"


namespace AABB {

  struct Rect {
    std::array<SDL_FPoint, 4> vertexes;
    std::array<bool, 4> moving{ false, false, false, false};
  };

  Rect Create(const Graphics::Center_Panel &panel);

}