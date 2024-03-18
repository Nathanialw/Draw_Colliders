//
// Created by nathanial on 3/9/24.
//
#include "iostream"

#include "SDL2/SDL.h"
#include "../Graphics/graphics.h"
#include "../App/core.h"
#include "../Graphics/text.h"
#include "../App/Settings/save.h"
#include "mouse.h"
#include "actions.h"

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
          if (Action::Add_Image(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_s) {
          if (Action::Save(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_l) {
          if (Action::Load(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_q) {
          if (Action::Close_Project(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_a) {
          if (Action::Add_Vertex(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_d) {
          if (Action::Delete_Shape(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_v) {
          if (Action::Delete_Vertex(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_y) {
          if (Action::Remove_Image(app))
            return true;
        }

        if (event.key.keysym.sym == SDLK_ESCAPE) {
          if (Action::Quit_Application(app))
            return true;
        }
        return true;
      }
    }
    return false;
  }
}