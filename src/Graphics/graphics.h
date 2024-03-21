#pragma once
//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL.h"
#include "string"
#include "vector"
#include "array"
#include "../../lib/SDL_FontCache/SDL_FontCache.h"

namespace Graphics {

  struct Image_Import {
    SDL_Texture* texture = nullptr;
    std::string fileName;
  };

  struct Image {
    SDL_Texture* texture = nullptr;
    float scale = 1.0f;
    SDL_FPoint offset = {0.0f, 0.0f};
  };

  struct Button {
    SDL_FRect button;
    SDL_Texture *texture;
    std::string label;
  };

  struct ScrollBar {
    SDL_FRect panel;
    SDL_FRect bar;
  };

  struct Left_Panel {
    SDL_FRect panel;
    SDL_FRect body;
    ScrollBar scroll;
    SDL_FRect search;
  };

  const int ButtonBarSize = 9;
  struct Button_Bar {
    SDL_FRect panel;
    std::array<Button, ButtonBarSize> buttons;
  };

  enum Shape {
    POINT,
    LINE,
    CIRCLE,
    AABB,
    POLYGON,
    SIZE,
  };

  struct Shapes {
    SDL_FRect panel{};
    std::array<std::vector<SDL_FRect>, Shape::SIZE> shapes;
    ScrollBar scroll{};
    SDL_FRect expanderLeft{};
    SDL_FRect body{};
  };

  struct Center_Panel {
    Button_Bar buttonBar{};
    SDL_FRect panel{};
    SDL_FRect image{};
    Shapes shapes{};
    SDL_FRect expanderLeft{};
    SDL_FRect expanderRight{};
  };

  struct Right_Panel {
    SDL_FRect panel;
    SDL_FRect body;
    ScrollBar scroll;

    //options
    SDL_FRect exportType;
    SDL_FRect fileName;
    SDL_FRect size; // in pixels

    SDL_FRect anchorPoint;
    SDL_FRect bodyLabel;

    SDL_FRect fixRotation;
    SDL_FRect linearDampening;
    SDL_FRect angularDampening;

    SDL_FRect bounce;
    SDL_FRect affectedByGravity;

    SDL_FRect isDynamic;
    SDL_FRect isBullet;
    SDL_FRect isSensor;
    SDL_FRect density;
    SDL_FRect restitution;
    SDL_FRect friction;
    SDL_FRect group;
    //bitmask stuff
  };

  struct Main_Panel {
    Left_Panel left{};
    Center_Panel center{};
    Right_Panel right{};
  };

  const int menuSize = 11;
  struct Top {
    SDL_FRect panel{};
    std::array<Button, menuSize> buttons{};
  };

  const int numMenuButtons = 6;
  struct Menu {
    SDL_FRect panel{};
    std::array<SDL_FRect, numMenuButtons> buttons{};
  };

  struct Panels {
    //width adjusts with screen width
    //height static with screen height
    Menu menu{};
    Top top;
    SDL_FRect center{};
    SDL_FRect bottom{};
    Main_Panel mainPanel;
  };

  struct Context {
    SDL_Renderer *renderer{};
    SDL_Window *window{};
    FC_Font *font;

    int width{};
    int height{};
    Uint32 flags{};
    const char* title{};
  };

  struct Texture {
    SDL_Texture* checkbox = nullptr;
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
    std::vector<std::array<std::vector<SDL_Texture*>, Graphics::SIZE>> shapes{};
    // imported images
    std::vector<SDL_Texture*> images;

    //maybe for the preview images on the lft
    std::vector<SDL_Texture*> smallImages;
  };

  struct UI_Panels {
    float window_h = 0.0f;
    float window_w = 0.0f;

    float scrollBarLeftHeight = 60.0f;
    float scrollBarRightHeight = 60.0f;
    float scrollBarFixturesHeight = 60.0f;

    float scrollBarLeftY = 0.0f;
    float scrollBarRightY = 0.0f;
    float scrollBarFixturesY = 0.0f;

    float leftPanelWidth = 200.0f;
    float rightPanelWidth = 250.0f;
    float shapeListWidth = 125.0f;

    //left
    float w = 40.0f;
    float h = 40.0f;
    float spacing = 5.0f;
    int numElement = 0;

    float bottomHeight = 25.0f;
    float filterHeight = 30.0f;
    float scrollWidth = 20.0f;
//    float scrollBarHeight = 60.0f;
    float expanderWidth = 10.0f;
    float space = 2.0f;
    float buttonBarHeight = 50.0f;
  };

  Panels Set_Panels (SDL_Window* window, UI_Panels &ui_panels);
  Context CreateWindowAndRenderer();
  SDL_Texture* Load_Texture(const Context &context, const std::string &filePath);
  Image_Import Load_Image(const Context &context);
  std::vector<Graphics::Image_Import> Load_Images(const Context &context);
  void Set_Render_Draw_Color(SDL_Renderer *renderer, const SDL_Color &color);
  void Set_Render_Draw_Color(SDL_Renderer *renderer, const int &r, const int &g, const int &b, const int &a);
  void Reset_Render_Draw_Color(SDL_Renderer *renderer);
  Texture Load_Icons(SDL_Renderer *renderer);
  void Show_Overlay(const Context &context);
  void Wait(bool &loaded);
  SDL_Texture* Render_Circle(SDL_Renderer *renderer, const float &x, const float &y, const float &r, const SDL_Color &shapeFill, const SDL_Color &edgeColor);
  SDL_Texture* Render_Polygon_AABB(SDL_Renderer *renderer, std::vector<double> &xPolygonPoints, std::vector<double> &yPolygonPoints, const int &w, const int &h, const SDL_Color &shapeFill);
}
