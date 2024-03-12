//
// Created by nathanial on 3/9/24.
//
#include "../Graphics/render.h"
#include "../Input/keyboard.h"
#include "../Input/window.h"
#include "../Input/mouse.h"

#include "../Panels/Center/center.h"

namespace App {
  SDL_Event event;
  bool wait = false;

  bool Input (App &app) {
    if (app.moveImage || app.moveVertex) {
      while (SDL_PollEvent(&event)) {
        if (Keyboard::Event(event, app))
          return true;
        if (Window::Event(event, app))
          return true;
        if (Mouse::Event(event, app))
          return true;
      }
    }
    else {
      int x = 0;
      while (SDL_WaitEvent(&event)) {
        if (Keyboard::Event(event, app))
          return true;
        if (Window::Event(event, app))
          return true;
        if (Mouse::Event(event, app))
          return true;
        if (Mouse::Motion(event, app, x))
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
      Render::Present(app);
    }
  }
}