//export module Graphics;
#pragma once
//
// Created by nathanial on 3/9/24.
//

#include "SDL2/SDL.h"
#include "string"
#include "vector"
#include "array"
#include "SDL_FontCache/SDL_FontCache.h"
//#include "../UI/scroll_bar.h"


namespace Graphics {

#define global_variable static;

//  dark indigo #2C2C54 44 44 84
//  deep purple-gray #474787 71 71 135
//  cool gray #AAABB8
//  light gray #ECECEC

  enum Color {
    DARK_BORDERS,
    SELECTED,
    BACKGROUND,
    LIGHTBACKGROUND,
    BORDERS,
    SHAPESELECTED,
    SHAPE,
    ICONBACKGROUND,
    ICON,
    BLACK,
    WHITE,
    SIZE,
  };

  static SDL_Color color[(int)Color::SIZE] = {
      {71, 71, 135, 255},     //purple-gray
      {170, 171, 184, 255},   //deep purple
      {100, 100, 135, 255},    //light
      {236, 236, 236, 255},   //light-gray
      {44, 44, 84, 255},      //dark indigo
      {0, 255, 255, 255},
      {200, 200, 184, 255},
      {255, 0, 0, 255},
      {130, 130, 135, 255},     //Icon back
      {0, 0, 0, 255},
      {200, 200, 200, 255},
  };


  struct ScrollBar {
    SDL_FRect panel{};
    SDL_FRect bar{};
    float elementHeight = 0.0f;
    float elementSpacing = 0.0f;
    bool show = false;
  };


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

  const int menuSize = 11;
  struct Top {
    SDL_FRect panel{};
    std::array<Button, menuSize> buttons{};
  };

  struct Menu {
    SDL_FRect panel{};
  };

  struct Right_Panel {
    SDL_FRect panel{};
    SDL_FRect body{};
    ScrollBar scroll;
  };


  struct Left_Panel {
    SDL_FRect panel{};
    SDL_FRect body{};
    ScrollBar scroll;
    SDL_FRect search{};
  };

  struct Shapes {
    SDL_FRect panel{};
//    std::array<std::vector<SDL_FRect>, SIZE> shapes;
    Graphics::ScrollBar scroll{};
    SDL_FRect expanderLeft{};
    SDL_FRect body{};
  };

  const int ButtonBarSize = 9;
  struct Button_Bar {
    SDL_FRect panel{};
    std::array<Graphics::Button, ButtonBarSize> buttons;
  };

  struct Center_Panel {
    Button_Bar buttonBar{};
    SDL_FRect panel{};
    SDL_FRect image{};
    Shapes shapes{};
    SDL_FRect expanderLeft{};
    SDL_FRect expanderRight{};
  };

  struct Main_Panel {
    Left_Panel left{};
    Center_Panel center{};
    Right_Panel right{};
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
    FC_Font *font{};

    int width{};
    int height{};
    Uint32 flags{};
    const char* title{};
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

  void SetRenderDrawColor(SDL_Renderer *renderer, const SDL_Color &color);

  Panels Set_Panels (SDL_Window* window, UI_Panels &ui_panels);
  Context CreateWindowAndRenderer();
  SDL_Texture* Load_Texture(const Context &context, const std::string &filePath);
  Image_Import Load_Image(const Context &context);
  std::vector<Graphics::Image_Import> Load_Images(const Context &context);
  void Set_Render_Draw_Color(SDL_Renderer *renderer, const SDL_Color &color);
  void Set_Render_Draw_Color(SDL_Renderer *renderer, const int &r, const int &g, const int &b, const int &a);
  void Reset_Render_Draw_Color(SDL_Renderer *renderer);
  void Show_Overlay(const Context &context);
  void Wait(bool &loaded);
  SDL_Texture* Render_Circle(SDL_Renderer *renderer, const float &x, const float &y, const float &r, const SDL_Color &shapeFill, const SDL_Color &edgeColor);
  SDL_Texture* Render_Polygon_AABB(SDL_Renderer *renderer, std::vector<double> &xPolygonPoints, std::vector<double> &yPolygonPoints, const int &w, const int &h, const SDL_Color &shapeFill);
}
