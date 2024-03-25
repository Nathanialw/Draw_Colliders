#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_DATA_H
#define BOX2D_COLLIDERS_DATA_H

#endif //BOX2D_COLLIDERS_DATA_H

#include "../Graphics/graphics.h"
//#include "Bounding_Boxes/polygon.h"
//#include "Bounding_Boxes/point.h"
//#include "Bounding_Boxes/aabb.h"
//#include "Bounding_Boxes/circle.h"
//#include "Bounding_Boxes/line_segment.h"
#include "Bounding_Boxes/shape.h"
//#include "../UI/scroll_bar.h"


namespace Data {
  union Option_Value {
    int numValue;
    bool isChecked;
  };

  enum Options {
    body_label,
    isDynamic,
    isSensor,
    isBullet,
    filter_category_bits,
    filter_mask_bits,
    bounce,
    density,
    group,
    restitution,
    friction,
    SIZE,
  };


  enum Option_Type {
    NONE,
    LABEL,
    CHECKBOX,
    NUMINPUT,
    TEXTINPUT
  };

  struct Option_Input {
//    Graphics::Button checkBox;
    Option_Type type;
  };



  struct Center {
    Graphics::Image texture;
    std::array<std::vector<Shape::Shape>, Shape::SIZE> shapes;
    std::array<Data::Options, Data::Options::SIZE> options{};

    int index = 0;
    //list to show/hide polygons
  };

  struct Left {
    std::vector<Center> images;
    std::vector<std::string> imageNameStr;

    int selected = 0;
    std::vector<int> filteredIndexes;
    std::vector<std::string> imagePathStr;
    std::vector<Graphics::Image> imageNames;
  };




  struct Right {
    std::array<std::string, 18> optionName;
    std::array<Data::Option_Value, 18> option{};
    std::array<Data::Option_Input, 18> optionInput{};
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

  struct App_Interface {
    Left left;
    Center center;
    Right right;
    Menu menu;
    Shape::Shape_List shapeList;
    App_Buttons appButtons;
    Edit_Buttons editButtons;
  };

}