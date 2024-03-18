#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_LINE_SEGMENT_H
#define BOX2D_COLLIDERS_LINE_SEGMENT_H

#endif //BOX2D_COLLIDERS_LINE_SEGMENT_H
#include "../../Graphics/graphics.h"
#include "shape.h"


namespace Line_Segment {

  Shape::Shape Create(const Graphics::Center_Panel &panel);
  Shape::Shape Create(const float &x, const float &y, const float &x2, const float &y2);
}