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
  struct Shape_List {
    std::array<std::vector<std::string>, Graphics::Shape::SIZE> shapeList;

    Shape_List () {
      shapeList[Graphics::POINT].emplace_back("Points");
      shapeList[Graphics::CIRCLE].emplace_back("Circles");
      shapeList[Graphics::LINE].emplace_back("Lines");
      shapeList[Graphics::AABB].emplace_back("AABBs");
      shapeList[Graphics::POLYGON].emplace_back("Polygons");
    }
  };


  struct Center {
    Graphics::Image texture;
    std::array<std::vector<Shape::Shape>, Graphics::SIZE> shapes;

    int index = 0;
    //list to show/hide polygons
  };

  struct Left {
    std::vector<Center> images;
    std::vector<std::string> imageNameStr;
    bool isSensor = false;
    int bounce = 0;
    int density = 0;
    int friction = 0;

    int selected = 0;
    std::vector<int> filteredIndexes;
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