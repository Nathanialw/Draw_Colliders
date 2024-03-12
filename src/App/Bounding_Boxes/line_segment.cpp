//
// Created by nathanial on 3/11/24.
//

#include "line_segment.h"


namespace Line_Segment {

  Line Create(const Graphics::Center_Panel &panel) {
    Line line{};
    line.vertexes[0] = {-50.0f, 0.0f};
    line.vertexes[1] = {50.0f, 0.0f};
    return line;
  }


}