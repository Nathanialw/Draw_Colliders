//
// Created by nathanial on 3/9/24.
//
#include "iostream"

#include "window.h"
#include "../Graphics/graphics.h"
#include "SDL2/SDL_image.h"
#include "../Utils/utils.h"
#include "../Graphics/text.h"

namespace Window {
  bool setFocus = true;

  bool Event(const SDL_Event &event, App::App &app) {
    if (event.type == SDL_DROPFILE) {
      auto texture = IMG_LoadTexture(app.context.renderer, event.drop.file);
      if (texture) {
        std::string fileName = event.drop.file;
        SDL_free(event.drop.file);

        Data::Center image;
        image.texture.texture = texture;
        image.index = app.interface.left.images.size();

        app.interface.left.images.emplace_back(image);
        app.interface.left.imageNameStr.emplace_back(Text::Get_File_Name(fileName));
        app.interface.left.imagePathStr.emplace_back(fileName);

        //load the image to the center if there is no image
        if (!app.interface.center.texture.texture)
          app.interface.center = image;

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