//
// Created by nathanial on 3/11/24.
//

#include "point.h"
#include "../../Graphics/graphics.h"


namespace Point {
  Point Create(const Graphics::Center_Panel &panel) {
    Point point = {
        0.0f,
        0.0f,
        false
    };
    return point;
  };
}