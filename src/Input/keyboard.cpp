//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL.h"
#include "../Graphics/graphics.h"
#include "../App/core.h"

namespace Keyboard {

  bool Event(const SDL_Event &event, App::App &app) {

    if (event.key.type == SDL_KEYDOWN) {
      if (event.key.keysym.sym == SDLK_o) {
        auto texture = Graphics::Load_Image(app.context);
        if (texture) {
          Graphics::Image image;
          image.texture = texture;
          app.interface.left.images.emplace_back(image);
        }
      }
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        app.running = !app.running;
      }
      return true;
    }

//    else if (event.key.type == SDL_KEYUP) {
//      return true;
//    }
    return false;
  }
}