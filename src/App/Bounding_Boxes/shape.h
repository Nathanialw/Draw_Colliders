#pragma once

//
// Created by nathanial on 3/18/24.
//

#ifndef BOX2D_COLLIDERS_SHAPE_H
#define BOX2D_COLLIDERS_SHAPE_H

#endif //BOX2D_COLLIDERS_SHAPE_H


namespace Shape {

  struct  Shape {
    std::vector<SDL_FPoint> vertices{};
    std::vector<bool> moving{};

    bool isSensor = false;
    bool isDynamic = false;
    bool isBullet = false;
    int bounce = 0;
    int density = 0;
    int friction = 0;
    int restitution = 0;

    int group = 0;
//    virtual bool Collision(Shape *shape) = 0;
  };

}