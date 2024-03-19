#pragma once
//
// Created by nathanial on 3/13/24.
//

#ifndef BOX2D_COLLIDERS_SAVE_H
#define BOX2D_COLLIDERS_SAVE_H

#endif //BOX2D_COLLIDERS_SAVE_H
#include "../core.h"


namespace Save {

  bool Save(App::App &app, const std::string &fileName);
  void Load(App::App &app, const std::string &fileName);
  bool Load_As(App::App &app);
  bool Save_As(App::App &app);
}
