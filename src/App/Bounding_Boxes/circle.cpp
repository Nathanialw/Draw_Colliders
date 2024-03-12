//
// Created by nathanial on 3/11/24.
//

#include "circle.h"

namespace Circle {
  Circle Create(const Graphics::Center_Panel &panel) {
    Circle circle{};
    circle.vertexes[0] = {0.0f, 0.0f};
    circle.vertexes[1] = {0.0f, 50.0f};
    circle.r = 50.0f;

    return circle;
  };

}