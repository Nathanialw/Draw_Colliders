#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_DATA_H
#define BOX2D_COLLIDERS_DATA_H

#endif //BOX2D_COLLIDERS_DATA_H

#include "../Graphics/graphics.h"
#include "Bounding_Boxes/polygon.h"
#include "Bounding_Boxes/point.h"
#include "Bounding_Boxes/aabb.h"
#include "Bounding_Boxes/circle.h"
#include "Bounding_Boxes/line_segment.h"

namespace Data {
  struct Center {
    Graphics::Image texture;
    //vertices and polygons
    std::array<std::vector<Shape::Shape>, Graphics::SIZE> shapes;
//    std::vector<Shape::Shape> circles;
//    std::vector<Shape::Shape> rects;
//    std::vector<Shape::Shape> lineSegments;
//    std::vector<Shape::Shape> polygons;
//    std::vector<Shape::Shape> points;
    int index = 0;
    //list to show/hide polygons
  };

  struct Left {
    int selected = 0;
    std::vector<Center> images;
    std::vector<std::string> imageNameStr;
    std::vector<std::string> imagePathStr;
    std::vector<Graphics::Image> imageNames;
  };


  struct Right {
  };

  struct Menu {
    std::array<Graphics::Button, 6> buttons;
  };

  struct App_Buttons {
    std::array<Graphics::Button, 6> buttons;
  };

  struct Edit_Buttons {
    std::array<Graphics::Button, 6> buttons;
  };
}