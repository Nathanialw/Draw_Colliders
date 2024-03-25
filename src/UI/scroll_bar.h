#pragma once
//
// Created by nathanial on 3/23/24.
//

#ifndef BOX2D_COLLIDERS_SCROLL_BAR_H
#define BOX2D_COLLIDERS_SCROLL_BAR_H

#endif //BOX2D_COLLIDERS_SCROLL_BAR_H
#include "../App/core.h"
#include "../Graphics/graphics.h"

namespace Scroll_Bar {

  struct Index {
    int min = 0;
    int max = 0;
  };

  bool Set_Bar_Size(const float &numElement, const int &num, const float &panelH, float &barH);
  Index Get_Min_Index(const float &scrollPanelX, const float &scrollH, const float &scrollBarY, const float &barH, const int &containerSize, const int &numElements);

  void Update(App::App &app, Graphics::ScrollBar &scrollBar, float &scrollBarHeight, const int &numElements,float &bodyW);
  bool Scroll(App::App &app, Graphics::ScrollBar &scrollBar, float &scrollBarPosition, float &scrollBarHeight, const int &numElements, const Sint32 &scroll, float &bodyW);
  void Update_UI_Position(App::App &app);
}