#pragma once
//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL_mouse.h"
#include "../Graphics/graphics.h"
#include "vector"

namespace App {
  struct Left {
    std::vector<Graphics::Image> images;
    std::vector<std::string> imageNameStr;
    std::vector<Graphics::Image> imageNames;
  };

  struct Center {
    Graphics::Image texture;
    //vertexes and polygons
    //list to show/hide polygons
  };

  struct Right {
  };

  struct Menu {
    std::array<Graphics::Button, 6> buttons;
  };

  struct App_Buttons {
    std::array<Graphics::Button, 6> buttons;
  };

  struct Edit_Buttons {
    std::array<Graphics::Button, 6> buttons;
  };

  struct App_Data {
    Left left;
    Center center;
    Right right;
    Menu menu;
    App_Buttons appButtons;
    Edit_Buttons editButtons;
  };

  struct App {
    Graphics::Panels panel;
    Graphics::Context context;
    App_Data appData;
    bool running = true;
    SDL_Cursor* cursor = nullptr;
    int max = 10;
    SDL_SystemCursor currentCursorId = SDL_SYSTEM_CURSOR_ARROW;
  };


  void Close(App &app);
  void Init (App &app);

}