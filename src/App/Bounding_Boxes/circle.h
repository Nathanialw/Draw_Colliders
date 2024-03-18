#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_CIRCLE_H
#define BOX2D_COLLIDERS_CIRCLE_H

#endif //BOX2D_COLLIDERS_CIRCLE_H

#include "../../Graphics/graphics.h"
#include "shape.h"

namespace Circle {

//  struct Circle {
//    //0 is vertex
//    //1 is radius "vertex"
//    std::array<SDL_FPoint, 2> vertices{};
//    std::array<bool, 2> moving{ false, false};
//  };

  Shape::Shape Create(const Graphics::Center_Panel &panel);
  Shape::Shape Create(const float &x, const float &y, const float &y2);
}