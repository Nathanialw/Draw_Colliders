#pragma once
//
// Created by nathanial on 17/03/24.
//

#ifndef BOX2D_COLLIDERS_ACTIONS_H
#define BOX2D_COLLIDERS_ACTIONS_H

#endif // BOX2D_COLLIDERS_ACTIONS_H

#include "../App/core.h"

namespace Action {

  bool Delete_Shape(App::App &app);
  bool Delete_Vertex(App::App &app);
  bool Add_Vertex(App::App &app);
  bool Add_Vertex_Center(App::App &app);
  bool Add_Image(App::App &app);
  bool Add_Images(App::App &app);
  bool Remove_Image(App::App &app);
  bool New_Project(App::App &app);
  bool Open_Project(App::App &app);
  bool Close_Project(App::App &app);
  bool Quit_Project(App::App &app);
  bool Quit_Application(App::App &app);
  bool Publish(App::App &app);
  bool Publish_As(App::App &app);
  bool Save(App::App &app);
  bool Save_As(App::App &app);
  bool Load(App::App &app);

  typedef int (*Button)(App::App &app);

  typedef Shape::Shape (*CREATE_SHAPE)();

  int Create_Circle(App::App &app);
  int Create_Point(App::App &app);
  int Create_Polygon(App::App &app);
  int Create_Rect(App::App &app);
  int Create_Line(App::App &app);
  int Delete_Selected_Shape(App::App &app);
  int Create_Vertex_If_Polygon_Selected(App::App &app);
  int Delete_Vertex_If_Polygon_Selected(App::App &app);
  int Unused(App::App &app);


}