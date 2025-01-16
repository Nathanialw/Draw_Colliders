#pragma once
//
// Created by nathanial on 3/9/24.
//
#include "vector"
#include "array"

#include "SDL2/SDL_mouse.h"

#include "data.h"
#include "Settings/serialise.h"
#include "../Utils/utils.h"
//#include "../Graphics/graphics.h"
//#include "Bounding_Boxes/shape.h"

//import Graphics;

namespace App {

//  typedef Length;

  struct Texture {
    SDL_Texture* checkedBox = nullptr;
    SDL_Texture* uncheckedBox = nullptr;
    SDL_Texture* deleteShape = nullptr;
    SDL_Texture* addVertex = nullptr;
    SDL_Texture* deleteVertex = nullptr;
    SDL_Texture* save = nullptr;
    SDL_Texture* saveAs = nullptr;
    SDL_Texture* location = nullptr;
    SDL_Texture* nodes = nullptr;
    SDL_Texture* pentagon = nullptr;
    SDL_Texture* point = nullptr;
    SDL_Texture* unchecked = nullptr;
    SDL_Texture* up = nullptr;
    SDL_Texture* vector = nullptr;
    SDL_Texture* view = nullptr;
    SDL_Texture* newDocument = nullptr;
    SDL_Texture* open = nullptr;
    SDL_Texture* show = nullptr;
    SDL_Texture* hide = nullptr;
    SDL_Texture* addFolder = nullptr;
    SDL_Texture* circle = nullptr;
    SDL_Texture* vertex = nullptr;
    SDL_Texture* publish = nullptr;
    SDL_Texture* publishAs = nullptr;
    SDL_Texture* deleteImage = nullptr;
    SDL_Texture* addImage = nullptr;
    SDL_Texture* alphaTexture = nullptr;



    //image index // shape type // shape index
    std::vector<std::array<std::vector<SDL_Texture*>, Shape::SIZE>> shapes{};
    // imported images
    std::vector<SDL_Texture*> images;

    //maybe for the preview images on the lft
    std::vector<SDL_Texture*> smallImages;
  };

  enum Text_Box_Selected {
    NONE,
    BODY,
    FILTER,
    FILTER_CATEGORY,
    FILTER_MASK,
    GROUP,
    FRICTION,
    BOUNCE,
    DENSITY,
    RESTITUTION,
  };

  struct App {
    Graphics::UI_Panels uiPanels;
    Graphics::Panels panel;
    Graphics::Context context;

    Data::App_Interface interface{};
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
    Shape::Fixture selectedShape;
    Shape::Vertex selectedVertex;
    Shape::Vertex vertex;

//    std::array<bool, SIZE> textBoxSelected;
    int selected = 0;
    float cachedScrollBarPosition = 0.0f;

    int imageIndex = 0;
    bool filterImages = false;
    std::string filterText = "Filter...";
    //set it to italic and greyed out
    std::string filterTextDefault = "Filter...";

    Serialise::Datafile datafile;
    Texture texture;
    std::string saveName;
  };


  void Close(App &app);
  void New(App &app);
  void Set_Textures(App &app);
  void Init (App &app);
  SDL_FPoint Offset_From_Image_Center(const App &app, const SDL_FPoint &point);
}