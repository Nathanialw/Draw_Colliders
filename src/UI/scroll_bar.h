#pragma once
//
// Created by nathanial on 3/23/24.
//

#ifndef BOX2D_COLLIDERS_SCROLL_BAR_H
#define BOX2D_COLLIDERS_SCROLL_BAR_H

#endif //BOX2D_COLLIDERS_SCROLL_BAR_H
#include "../App/core.h"

namespace Scroll_Bar {


  bool Scroll(App::App &app, const Graphics::ScrollBar &scrollBar, float &scrollBarPosition, float &scrollBarHeight, const int &numElements, const Sint32 &scroll);
  void Update_UI_Position(App::App &app);
}