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

namespace Polygon {

  struct Polygon {
    std::vector<SDL_FPoint> vertexes{};
    std::vector<bool> moving{};

  };

  Polygon Create(const Graphics::Center_Panel &panel);
}