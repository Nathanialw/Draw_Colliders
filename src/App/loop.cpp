//
// Created by nathanial on 3/9/24.
//
#include "../Graphics/render.h"
#include "../Input/keyboard.h"
#include "../Input/window.h"
#include "../Input/mouse.h"

#include "../Panels/Center/center.h"
#include "../Panels/menu.h"

namespace App {
  SDL_Event event;
  bool wait = false;

  bool Input (App &app) {
    if (!app.wait || app.moveImage || app.moveVertex) {
      if (!app.wait) {
        app.timer++;
        if (app.timer > 1000) {
          app.timer = 0;
          app.wait = true;
        }
      }

      while (SDL_PollEvent(&event)) {
        if (Window::Event(event, app))
          return true;
        if (Mouse::Event(event, app))
          return true;
        if (Keyboard::Event(event, app))
          return true;
      }
    }
    else {
      int x = 0;
      while (SDL_WaitEvent(&event)) {
        if (Window::Event(event, app))
          return true;
        if (Mouse::Event(event, app))
          return true;
        if (Mouse::Motion(event, app, x))
          return true;
        if (Keyboard::Event(event, app))
          return true;
      }
    }
    return false;
  }

  void Run(App &app) {
    Render::Present(app);
    while (app.running) {
      Input(app);
      ::Center::Center::Update_Image(app);
      Mouse::Update_Cursor(app);
      Menu::Update(app);
      Render::Present(app);
    }
  }
}