//
// Created by nathanial on 3/11/24.
//

#include "circle.h"
#include "shape.h"

namespace Circle {
  Shape::Shape Create(const Graphics::Center_Panel &panel) {
    Shape::Shape circle{};
    circle.vertices.resize(2);
    circle.vertices[0] = {0.0f, 0.0f};
    circle.vertices[1] = {0.0f, 50.0f};
    circle.moving.resize(2);
    circle.moving[0] = false;
    circle.moving[1] = false;

    return circle;
  };

  Shape::Shape Create(const float &x, const float &y, const float &y2) {
    Shape::Shape circle{};
    circle.vertices.resize(2);
    circle.vertices[0] = {x, y};
    circle.vertices[1] = {x, y2};
    circle.moving.resize(2);
    circle.moving[0] = false;
    circle.moving[1] = false;

    return circle;
  };

}