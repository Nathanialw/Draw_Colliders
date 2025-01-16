#pragma once

//
// Created by nathanial on 3/10/24.
//

#ifndef BOX2D_COLLIDERS_LEFT_H
#define BOX2D_COLLIDERS_LEFT_H

#endif //BOX2D_COLLIDERS_LEFT_H

#include "../../App/core.h"
namespace Center::Left {

  bool Hover_Image(App::App &app);
  Data::Center Select_Image(App::App &app);
  bool Set_Image(App::App &app);
  void Render(App::App &app);
  bool Scroll(App::App &app, const Sint32 &scroll);
  bool Filter_Images(App::App &app);

}