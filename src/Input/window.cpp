//
// Created by nathanial on 3/9/24.
//

#include "window.h"
#include "../Graphics/graphics.h"
#include "SDL2/SDL_image.h"

namespace Window {
  bool setFocus = true;

  bool Event(const SDL_Event &event, App::App &app) {
    if (event.type == SDL_DROPFILE) {
      auto texture = IMG_LoadTexture(app.context.renderer, event.drop.file);
      if (texture) {
        Graphics::Image image;
        image.texture = texture;
        app.appData.left.images.emplace_back(image);
        if (setFocus)
          SDL_SetWindowInputFocus(app.context.window);
        return true;
      }
    }

    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
      app.panel = Graphics::Set_Panels(app.context.window);
      return true;
    }

    if (event.type == SDL_QUIT) {
      app.running = !app.running;
    }

    return false;
  }

}