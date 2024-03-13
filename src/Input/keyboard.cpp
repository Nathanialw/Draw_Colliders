//
// Created by nathanial on 3/9/24.
//
#include "iostream"

#include "SDL2/SDL.h"
#include "../Graphics/graphics.h"
#include "../App/core.h"
#include "../Graphics/text.h"
#include "../App/Settings/save.h"

namespace Keyboard {


  bool Event(const SDL_Event &event, App::App &app) {

    //filter box
    if (app.filterImages) {
      if (event.type == SDL_TEXTINPUT) {
        app.filterText += event.text.text;
        return true;
      }

      else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_BACKSPACE: {
            if (!app.filterText.empty())
              app.filterText = app.filterText.erase(app.filterText.size() - 1);
            return true;
          }
          case SDLK_ESCAPE: {
            app.filterText = app.filterTextDefault;
            app.filterImages = false;
            return true;
          }
        }
      }
      return true;
    }

    //application hotkeys
    else {
      if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_o) {
          auto imageImport = Graphics::Load_Image(app.context);
          if (imageImport.texture) {
            Data::Center image;
            image.texture.texture = imageImport.texture;
            image.index = app.interface.left.images.size();

            app.interface.left.images.emplace_back(image);
            app.interface.left.imageNameStr.emplace_back(Text::Get_File_Name(imageImport.fileName));
            app.interface.left.imagePathStr.emplace_back(imageImport.fileName);

            //load the image to the center if there is no image
            if (!app.interface.center.texture.texture)
              app.interface.center = image;
          }
        }

        if (event.key.keysym.sym == SDLK_s) {
          Save::State(app);
          app.datafile.Write(app.datafile, "save.txt");
        }

        if (event.key.keysym.sym == SDLK_l) {
          Save::Load(app);
//          app.datafile.Read(app.datafile, "save.txt");
        }

        if (event.key.keysym.sym == SDLK_ESCAPE) {
          app.running = !app.running;
        }
        return true;
      }
    }
    return false;
  }
}