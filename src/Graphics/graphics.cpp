//
// Created by nathanial on 3/9/24.
//
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "iostream"
#include "graphics.h"
#include "../../lib/nativefiledialog/nfd.h"

namespace Graphics {
  using namespace Graphics;

  struct Window {
    int w;
    int h;
  };

  Context CreateWindowAndRenderer() {
    Context context;
    context.width = 1920;
    context.height = 1080;
    context.flags = SDL_WINDOW_RESIZABLE;
    context.title = "Collider";
    context.window = SDL_CreateWindow(context.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, context.width, context.height, context.flags);
//    context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
    context.renderer = SDL_CreateRenderer(context.window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
    return context;
  }

  Window GetWindowSize(SDL_Window* window) {
    Window windowSize{};
    SDL_GetWindowSize(window, &windowSize.w, &windowSize.h);
    return windowSize;
  }

  void Set_Render_Draw_Color(SDL_Renderer *renderer, const SDL_Color &color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
  }

  void Set_Render_Draw_Color(SDL_Renderer *renderer, const int &r, const int &g, const int &b, const int &a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
  }
  void Reset_Render_Draw_Color(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  }

  void Update() {

  }

  float Cap(const float &panelHeight, float h) {
    if (h > panelHeight)
      return panelHeight;
    else
      return  h;
  }

  float bottomHeight = 25.0f;
  float rightPanelWidth = 200.0f;
  float leftPanelWidth = 200.0f;
  float filterHeight = 30.0f;
  float scrollWidth = 20.0f;
  float scrollBarHeight = 60.0f;
  float expanderWidth = 10.0f;
  float shapeListWidth = 125.0f;
  float space = 2.0f;
  float buttonBarHeight = 50.0f;

  Panels Set_Panels (SDL_Window* window) {
    Panels panels{};
    float btn_y;

    Window windowSize = GetWindowSize(window);
    panels.menu.panel = {0.0f, 0.0f, (float)windowSize.w, 25.0f};
    panels.top = {0.0f, panels.menu.panel.h, (float)windowSize.w, 75.0f};

    float topButtonHeight = Cap(panels.top.panel.h, 50.0f);
    btn_y = panels.top.panel.y + (panels.top.panel.h - topButtonHeight) / 2.0f;
    for (int i = 0; i < panels.top.buttons.size(); ++i) {
      float btn_x = panels.top.panel.x + ((float)i * topButtonHeight) + (((panels.top.panel.h - topButtonHeight) / 2.0f) * ((float)i + 1.0f)) ;
      panels.top.buttons[i] = {btn_x, btn_y, topButtonHeight, topButtonHeight};
    }

    panels.bottom = {0.0f, (float)windowSize.h - bottomHeight, (float)windowSize.w, bottomHeight};
    panels.center = {0.0f, panels.menu.panel.h + panels.top.panel.h, (float)windowSize.w, (float)windowSize.h - (panels.menu.panel.h + panels.top.panel.h + panels.bottom.h)};

    //left
    panels.mainPanel.left.panel = {0.0f, panels.center.y, leftPanelWidth, panels.center.h};
    panels.mainPanel.left.search = {panels.mainPanel.left.panel.x, panels.mainPanel.left.panel.y, leftPanelWidth, filterHeight};
    panels.mainPanel.left.body = {panels.mainPanel.left.panel.x, panels.mainPanel.left.panel.y + filterHeight, leftPanelWidth - scrollWidth, panels.center.h - filterHeight};
    panels.mainPanel.left.scroll.panel = {panels.mainPanel.left.panel.x + panels.mainPanel.left.panel.w - scrollWidth, panels.mainPanel.left.panel.y + filterHeight, scrollWidth, panels.mainPanel.left.panel.h - filterHeight};
    panels.mainPanel.left.scroll.bar = {panels.mainPanel.left.panel.x + panels.mainPanel.left.panel.w - scrollWidth, panels.mainPanel.left.panel.y + filterHeight, scrollWidth, scrollBarHeight};

    //right
    panels.mainPanel.right.panel = {panels.center.w - rightPanelWidth, panels.center.y, rightPanelWidth, panels.center.h};
    panels.mainPanel.right.body = {panels.mainPanel.right.panel.x, panels.mainPanel.right.panel.y, rightPanelWidth - scrollWidth, panels.center.h};
    panels.mainPanel.right.scroll.panel = {panels.mainPanel.right.panel.x + panels.mainPanel.right.panel.w - scrollWidth, panels.mainPanel.right.panel.y, scrollWidth, panels.mainPanel.right.panel.h};
    panels.mainPanel.right.scroll.bar = {panels.mainPanel.right.panel.x + panels.mainPanel.right.panel.w - scrollWidth, panels.mainPanel.right.panel.y, scrollWidth, scrollBarHeight};

    //center
    panels.mainPanel.center.panel = {panels.mainPanel.left.panel.w + expanderWidth, panels.center.y, panels.center.w - (panels.mainPanel.left.panel.w + panels.mainPanel.right.panel.w) - (expanderWidth * 2.0f), panels.center.h};

    panels.mainPanel.center.buttonBar = {panels.mainPanel.center.panel.x, panels.mainPanel.center.panel.y, panels.mainPanel.center.panel.w, buttonBarHeight};
    float EditButtonHeight = Cap(panels.mainPanel.center.buttonBar.panel.h, 40.0f);
    btn_y = panels.mainPanel.center.buttonBar.panel.y + (panels.mainPanel.center.buttonBar.panel.h - EditButtonHeight) / 2.0f;
    for (int i = 0; i < panels.mainPanel.center.buttonBar.buttons.size(); ++i) {
      float btn_x = panels.mainPanel.center.buttonBar.panel.x + ((float)i * EditButtonHeight) + (((panels.mainPanel.center.buttonBar.panel.h - EditButtonHeight) / 2.0f) * ((float)i + 1.0f)) ;
      panels.mainPanel.center.buttonBar.buttons[i] = {btn_x, btn_y, EditButtonHeight, EditButtonHeight};
    }
    panels.mainPanel.center.expanderLeft = {panels.mainPanel.center.panel.x - expanderWidth, panels.mainPanel.center.panel.y, expanderWidth, panels.mainPanel.center.panel.h};
    panels.mainPanel.center.expanderRight = {(panels.mainPanel.center.panel.x + panels.mainPanel.center.panel.w), panels.mainPanel.center.panel.y, expanderWidth, panels.mainPanel.center.panel.h};
    panels.mainPanel.center.shapes.scroll.panel = {(panels.mainPanel.center.panel.x + panels.mainPanel.center.panel.w) - scrollWidth, panels.mainPanel.center.panel.y + buttonBarHeight, scrollWidth, panels.mainPanel.center.panel.h - buttonBarHeight};
    panels.mainPanel.center.shapes.scroll.bar = {(panels.mainPanel.center.panel.x + panels.mainPanel.center.panel.w) - scrollWidth, panels.mainPanel.center.panel.y + buttonBarHeight, scrollWidth, scrollBarHeight};
    panels.mainPanel.center.shapes.panel = {(panels.mainPanel.center.panel.x + panels.mainPanel.center.panel.w) - shapeListWidth, panels.mainPanel.center.panel.y + buttonBarHeight, shapeListWidth - scrollWidth, panels.mainPanel.center.panel.h - buttonBarHeight};
    panels.mainPanel.center.image = {panels.mainPanel.center.panel.x + expanderWidth + space, panels.mainPanel.center.panel.y + space + buttonBarHeight, panels.mainPanel.center.panel.w - shapeListWidth - ((expanderWidth * 2.0f) + (space * 2.0f)), panels.mainPanel.center.panel.h - (space * 2.0f) - buttonBarHeight};
    return panels;
  }

  SDL_Texture* Load_Texture(const Context &context, const std::string &filePath) {
    return IMG_LoadTexture(context.renderer, filePath.c_str());
  }

  Image_Import Load_Image(const Context &context) {
    nfdchar_t *outPath = nullptr;
    nfdresult_t result = NFD_OpenDialog("png,jpg;pdf", nullptr, &outPath );

    const char * filePath;
    if ( result == NFD_CANCEL ||  result == NFD_ERROR ) {
      free(outPath);
      return {nullptr, ""};
    }
    else if ( result == NFD_OKAY ) {
      filePath = outPath;
    }
    SDL_Texture* texture = IMG_LoadTexture(context.renderer, filePath);
//    std::cout << SDL_GetError() << std::endl;

    std::string fileName;
    if ( result == NFD_OKAY ) {
      // std::cout will not work from application, only from IDE or terminal
      /* Generally, applications run from terminal have direct access to STDIN/STDOUT/STDERR, whereas non-terminal-based applications do not.
       * */
//      puts("Success!");
//      puts(outPath);
      fileName = outPath;
      free(outPath);
    }
    return {texture, fileName};
  }
}