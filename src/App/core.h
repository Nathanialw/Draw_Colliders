#pragma once
//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL_mouse.h"
#include "../Graphics/graphics.h"
#include "vector"
#include "data.h"


namespace App {

  struct App_Interface {
    Data::Left left;
    Data::Center center;
    Data::Right right;
    Data::Menu menu;
    Data::App_Buttons appButtons;
    Data::Edit_Buttons editButtons;
  };

  struct Offsets {
    float x = 0.0f;
    float y = 0.0f;
    float r = 0.0f;
  };

  struct Vertex {
    Graphics::Shape shape = Graphics::AABB;
    int indexPolygon = 0;
    int indexVertex = 0;
  };

  struct App {
    Graphics::Panels panel;
    Graphics::Context context;

    App_Interface interface;
    float vertexRadius = 5.0f;

    bool running = true;
    SDL_Cursor* cursor = nullptr;
    int max = 10;
    SDL_SystemCursor currentCursorId = SDL_SYSTEM_CURSOR_ARROW;

    SDL_Point initialPosition{};
    SDL_Point offset = {0, 0};
    bool moveImage = false;
    bool moveVertex = false;
    Vertex vertex;

    bool filterImages = false;
    std::string filterText = "Filter...";
    //set it to italic and greyed out
    std::string filterTextDefault = "Filter...";
  };

  void Close(App &app);
  void Init (App &app);
  Offsets Calc_Offset(const App &app);
  SDL_FRect Vertex_To_Rect(const App &app, const SDL_FPoint &vertex, const Offsets &o, bool moveVertex);

  Vertex Get_Vertex(App &app, const SDL_FRect &cursor);

}