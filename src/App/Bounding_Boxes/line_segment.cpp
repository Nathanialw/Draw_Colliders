//
// Created by nathanial on 3/11/24.
//

#include "line_segment.h"


namespace Line_Segment {

  Shape::Shape Create() {
    Shape::Shape line{};
    line.vertices.push_back({-50.0f, 0.0f});
    line.vertices.push_back({50.0f, 0.0f});
    line.moving.push_back(false);
    line.moving.push_back(false);
    return line;
  }


  Shape::Shape Create(const float &x, const float &y, const float &x2, const float &y2) {
    Shape::Shape line{};
    line.vertices.push_back({x, y});
    line.vertices.push_back({x2, y2});
    line.moving.push_back(false);
    line.moving.push_back(false);
    return line;
  }

}