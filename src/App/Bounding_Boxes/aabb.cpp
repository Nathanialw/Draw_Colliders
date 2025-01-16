//
// Created by nathanial on 3/11/24.
//
#include "aabb.h"

namespace AABB {

  Shape::Shape Create() {
    Shape::Shape rect{};
    SDL_FPoint vertex;
    float radius = 50.0f;
    //topLeft
    vertex.x = -radius;
    vertex.y = -radius;
    rect.vertices.push_back(vertex);
    //topRight
    vertex.x = +radius;
    vertex.y = -radius;
    rect.vertices.push_back(vertex);
    //bottomRight
    vertex.x = +radius;
    vertex.y = +radius;
    rect.vertices.push_back(vertex);
    //bottomLeft
    vertex.x = -radius;
    vertex.y = +radius;
    rect.vertices.push_back(vertex);
    rect.moving.push_back(false);
    rect.moving.push_back(false);
    rect.moving.push_back(false);
    rect.moving.push_back(false);

    return rect;
  };

//  class AABB : public Shape::Shape {
//    bool Collision(Shape *shape) override {
//
//    }
//
//  };

}