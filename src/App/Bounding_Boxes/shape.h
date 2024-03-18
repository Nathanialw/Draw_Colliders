#pragma once

//
// Created by nathanial on 3/18/24.
//

#ifndef BOX2D_COLLIDERS_SHAPE_H
#define BOX2D_COLLIDERS_SHAPE_H

#endif //BOX2D_COLLIDERS_SHAPE_H


namespace Shape {

  class Shape {
  public:
    std::vector<SDL_FPoint> vertices{};
    std::vector<bool> moving{};

//    virtual bool Collision(Shape *shape) = 0;
  };

}