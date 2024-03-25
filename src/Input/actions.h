#pragma once
//
// Created by nathanial on 17/03/24.
//

#ifndef BOX2D_COLLIDERS_ACTIONS_H
#define BOX2D_COLLIDERS_ACTIONS_H

#endif // BOX2D_COLLIDERS_ACTIONS_H

#include "../App/core.h"

namespace Action {

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

  int Delete_Shape(App::App &app);
  int Delete_Vertex(App::App &app);
  int Add_Vertex(App::App &app);
  int Add_Vertex_Center(App::App &app);
  int Add_Image(App::App &app);
  int Add_Images(App::App &app);
  int Remove_Image(App::App &app);
  int New_Project(App::App &app);
  int Open_Project(App::App &app);
  int Close_Project(App::App &app);
  int Quit_Application(App::App &app);
  int Publish(App::App &app);
  int Publish_As(App::App &app);
  int Save(App::App &app);
  int Save_As(App::App &app);
  int Load(App::App &app);

}