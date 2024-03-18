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
  bool New_Project(App::App &app);
  bool Close_Project(App::App &app);
  bool Quit_Project(App::App &app);
  bool Quit_Application(App::App &app);
  bool Save(App::App &app);
  bool Load(App::App &app);

}