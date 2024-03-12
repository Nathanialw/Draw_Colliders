#pragma once

//
// Created by nathanial on 3/10/24.
//

#ifndef BOX2D_COLLIDERS_CENTER_H
#define BOX2D_COLLIDERS_CENTER_H

#endif //BOX2D_COLLIDERS_CENTER_H

#include "../../App/core.h"

namespace Center::Center {
  typedef int (*Edit_Button)(App::App &app);
  int Create_Circle_Button(App::App &app);
  int Create_Point_Button(App::App &app);
  int Create_Polygon_Button(App::App &app);
  int Create_Rect_Button(App::App &app);
  int Create_Line_Button(App::App &app);
  int Unused(App::App &app);
  int Click_Button(App::App &, const int &i);

  bool Set_Scale(App::App &app, Sint32 direction);
  bool Move(App::App &app);
  bool Move_Vertex(App::App &app);
  bool Set(App::App &app, SDL_FPoint &point);
  bool Set_Vertex(App::App &app);
  bool Update_Image(App::App &app);
  bool Set_Edit_Image(App::App &app);
  void Render(App::App &app);
}