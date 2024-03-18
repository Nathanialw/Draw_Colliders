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

  const int ButtonBarSize = 8;
  struct Button_Bar {
    SDL_FRect panel;
    std::array<SDL_FRect, ButtonBarSize> buttons;
  };

  enum Shape {
    AABB,
    POLYGON,
    CIRCLE,
    LINE,
    POINT,
    SIZE,
  };

  struct Shapes {
    SDL_FRect panel{};
    std::array<std::vector<SDL_FRect>, Shape::SIZE> shapes;
    ScrollBar scroll{};
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

  struct Top {
    SDL_FRect panel{};
    std::array<SDL_FRect, 6> buttons{};
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

  Panels Set_Panels(SDL_Window* window);
  Context CreateWindowAndRenderer();
  SDL_Texture* Load_Texture(const Context &context, const std::string &filePath);
  Image_Import Load_Image(const Context &context);
  void Set_Render_Draw_Color(SDL_Renderer *renderer, const SDL_Color &color);
  void Set_Render_Draw_Color(SDL_Renderer *renderer, const int &r, const int &g, const int &b, const int &a);
  void Reset_Render_Draw_Color(SDL_Renderer *renderer);
}
