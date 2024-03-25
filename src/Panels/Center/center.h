#pragma once

//
// Created by nathanial on 3/10/24.
//

#ifndef BOX2D_COLLIDERS_CENTER_H
#define BOX2D_COLLIDERS_CENTER_H

#endif //BOX2D_COLLIDERS_CENTER_H

#include "../../App/core.h"

namespace Center::Center {
  int Click_Button(App::App &, const int &i);

  bool Set_Scale(App::App &app, Sint32 direction);
  bool Move(App::App &app);
  bool Move_Vertex(App::App &app);
  bool Set(App::App &app, SDL_FPoint &point);
  bool Set_Vertex(App::App &app);
  bool Update_Image(App::App &app);

  bool Scroll(App::App &app, const Sint32 &scroll);
  App::Shape Select_From_Shape_List_Names(App::App &app);

  void Set_Expander(App::App &app);

  void Render(App::App &app);
}