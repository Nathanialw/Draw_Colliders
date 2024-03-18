//
// Created by nathanial on 3/11/24.
//

#include "polygon.h"

namespace Polygon {

  Shape::Shape Create(const Graphics::Center_Panel &panel) {
    Shape::Shape polygon{};
    SDL_FPoint vertex;
    float radius = 50.0f;

    //topLeft
    vertex.x = -radius;
    vertex.y = -radius;
    polygon.vertices.emplace_back(vertex);
    polygon.moving.emplace_back(false);
    //bottomRight
    vertex.x = +radius;
    vertex.y = +radius;
    polygon.vertices.emplace_back(vertex);
    polygon.moving.emplace_back(false);
    //bottomLeft
    vertex.x = -radius;
    vertex.y = +radius;
    polygon.vertices.emplace_back(vertex);
    polygon.moving.emplace_back(false);
    return polygon;
  }


//  Shape::Shape Create(const float &x, const float &y) {
//    Shape::Shape polygon{};
//    SDL_FPoint vertex;
//    float radius = 50.0f;
//
//    //topLeft
//    vertex.x = -radius;
//    vertex.y = -radius;
//    polygon.vertices.emplace_back(vertex);
//    polygon.moving.emplace_back(false);
//    //bottomRight
//    vertex.x = +radius;
//    vertex.y = +radius;
//    polygon.vertices.emplace_back(vertex);
//    polygon.moving.emplace_back(false);
//    //bottomLeft
//    vertex.x = -radius;
//    vertex.y = +radius;
//    polygon.vertices.emplace_back(vertex);
//    polygon.moving.emplace_back(false);
//    return polygon;
//  }

}