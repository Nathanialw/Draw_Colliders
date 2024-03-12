#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_LINE_SEGMENT_H
#define BOX2D_COLLIDERS_LINE_SEGMENT_H

#endif //BOX2D_COLLIDERS_LINE_SEGMENT_H
#include "../../Graphics/graphics.h"


namespace Line_Segment {

  struct Line {
    std::array<SDL_FPoint, 2> vertexes;
    std::array<bool, 2> moving{ false, false};
  };

  Line Create(const Graphics::Center_Panel &panel);
}