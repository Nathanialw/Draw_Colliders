//
// Created by nathanial on 3/11/24.
//
#include "aabb.h"

namespace AABB {

  Rect Create(const Graphics::Center_Panel &panel) {
    Rect rect{};
    SDL_FPoint vertex;
    float radius = 50.0f;

    //topLeft
    vertex.x = -radius;
    vertex.y = -radius;
    rect.vertexes[0] = vertex;
    //topRight
    vertex.x = +radius;
    vertex.y = -radius;
    rect.vertexes[1] = vertex;
    //bottomRight
    vertex.x = +radius;
    vertex.y = +radius;
    rect.vertexes[2] = vertex;
    //bottomLeft
    vertex.x = -radius;
    vertex.y = +radius;
    rect.vertexes[3] = vertex;

    return rect;
  };

}