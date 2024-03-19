#pragma once
//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL_mouse.h"
#include "../Graphics/graphics.h"
#include "vector"
#include "array"
#include "data.h"
#include "Settings/serialise.h"
#include "../Utils/utils.h"

namespace App {

  struct App_Interface {
    Data::Left left;
    Data::Center center;
    Data::Right right;
    Data::Menu menu;
    Data::Shape_List shapeList;
    Data::App_Buttons appButtons;
    Data::Edit_Buttons editButtons;
  };

  struct Offsets {
    float x = 0.0f;
    float y = 0.0f;
    float r = 0.0f;
  };

  struct Vertex {
    Graphics::Shape shape = Graphics::SIZE;
    int indexPolygon = 0;
    int indexVertex = 0;
  };

  struct Shape {
    Graphics::Shape shape = Graphics::SIZE;
    int indexPolygon = 0;
  };

  enum Mouse_Selected {
    SHAPE,
    VERTEX,
    VERTEXLIST,
    IMAGELIST,
    MENU
//    FILTERBOX ?? maybe
  };

  struct App {
    Graphics::Panels panel;
    Graphics::Context context;

    App_Interface interface{};
    float vertexRadius = 5.0f;

    bool running = true;
    SDL_Cursor* cursor = nullptr;
    int max = 10;
    int timer = 0;
    bool wait = true;
    SDL_SystemCursor currentCursorId = SDL_SYSTEM_CURSOR_ARROW;
    bool zoomToMouse = true;

    bool menuOpen = false;
    SDL_FRect menu{};

    SDL_Point initialPosition = {0, 0};
    SDL_FPoint offset = {0.0f, 0.0f};
    bool moveImage = false;
    bool moveVertex = false;
    Shape selectedShape;
    Vertex selectedVertex;
    Vertex vertex;

    int imageIndex = 0;
    bool filterImages = false;
    std::string filterText = "Filter...";
    //set it to italic and greyed out
    std::string filterTextDefault = "Filter...";

    Serialise::Datafile datafile;
    Graphics::Texture texture;
    std::string saveName;
  };

  void Close(App &app);
  void New(App &app);
  void Set_Textures(App &app);
  void Init (App &app);
  Offsets Calc_Offset(const App &app);
  SDL_FPoint Offset_From_Image_Center(const App &app, const SDL_FPoint &point);
  SDL_FRect Vertex_To_Rect(const App &app, const SDL_FPoint &vertex, const Offsets &o, bool moveVertex);

  Vertex Get_Vertex(App &app, const SDL_FRect &cursor);
  Vertex Get_Shape(App &app, const SDL_FRect &cursor);

}