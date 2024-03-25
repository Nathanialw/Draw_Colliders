#pragma once

//
// Created by nathanial on 3/10/24.
//

#ifndef BOX2D_COLLIDERS_MENU_H
#define BOX2D_COLLIDERS_MENU_H

#endif //BOX2D_COLLIDERS_MENU_H
#include "../App/core.h"

namespace Menu {

  bool SubMenu_Select(App::App &app);
  void Init(App::App &app);
  bool Is_Open();

  void Clear(App::App& app);
  bool Open(App::App& app);
  void Update(App::App& app);
  void Render(App::App &app);
}