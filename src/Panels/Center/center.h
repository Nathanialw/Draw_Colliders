#pragma once

//
// Created by nathanial on 3/10/24.
//

#ifndef BOX2D_COLLIDERS_CENTER_H
#define BOX2D_COLLIDERS_CENTER_H

#endif //BOX2D_COLLIDERS_CENTER_H

#include "../../App/core.h"

namespace Center::Center {
  bool isMoving();
  bool Set_Scale(App::App &app, Sint32 direction);
  bool Move(App::App &app);
  bool Set(App::App &app);
  bool Update_Image();
  bool Set_Edit_Image(App::App &app);
  void Render(App::App &app);
}