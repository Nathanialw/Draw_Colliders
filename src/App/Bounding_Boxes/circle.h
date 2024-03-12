#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_CIRCLE_H
#define BOX2D_COLLIDERS_CIRCLE_H

#endif //BOX2D_COLLIDERS_CIRCLE_H

#include "../../Graphics/graphics.h"

namespace Circle {

  struct Circle {
    //0 is vertex
    //1 is radius "vertex"
    std::array<SDL_FPoint, 2> vertexes{};
    std::array<bool, 2> moving{ false, false};
    float r = 50.0f;
  };

  Circle Create(const Graphics::Center_Panel &panel);
}