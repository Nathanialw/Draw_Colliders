//
// Created by nathanial on 3/11/24.
//

#include "point.h"
#include "../../Graphics/graphics.h"


namespace Point {
  Shape::Shape Create(const Graphics::Center_Panel &panel) {
    Shape::Shape point;
    point.vertices.push_back({0.0f, 0.0f });
    point.moving.push_back(false);
    return point;
  };

  Shape::Shape Create(const float &x, const float &y) {
    Shape::Shape point;
    point.vertices.push_back({x, y });
    point.moving.push_back(false);
    return point;
  };
}