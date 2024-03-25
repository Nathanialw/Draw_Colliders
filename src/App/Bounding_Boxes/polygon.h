#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_POLYGON_H
#define BOX2D_COLLIDERS_POLYGON_H

#endif //BOX2D_COLLIDERS_POLYGON_H
#include "vector"

#include "line_segment.h"
#include "../../Graphics/graphics.h"
#include "shape.h"

namespace Polygon {

  Shape::Shape Create();

  void Render_Polygon(SDL_Renderer *renderer, const std::vector<SDL_FPoint> &points, const SDL_Color &shapeFill);
}