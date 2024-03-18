#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_POINT_H
#define BOX2D_COLLIDERS_POINT_H

#endif //BOX2D_COLLIDERS_POINT_H

#include "../../Graphics/graphics.h"
#include "shape.h"


namespace Point {

  Shape::Shape Create(const Graphics::Center_Panel &panel);
  Shape::Shape Create(const float &x, const float &y);
}