//
// Created by nathanial on 3/9/24.
//
//import Graphics.

#ifdef __linux__
#include <SDL2/SDL.h>
#elif defined(_WIN32)
#include <SDL2/SDL.h>
#endif
#include <thread>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "iostream"
#include <limits>
#include "graphics.h"
#include "nativefiledialog/nfd.h"
#include "SDL2_gxf/SDL2_gfxPrimitives.h"


namespace Graphics {

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
    SDL_SetRenderDrawBlendMode(context.renderer, SDL_BLENDMODE_BLEND);

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

  float Cap(const float &panelHeight, float h) {
    if (h > panelHeight)
      return panelHeight;
    else
      return  h;
  }

  Panels Set_Panels (SDL_Window* window, UI_Panels &ui_panels) {
    Panels panels{};
    float btn_y;

    Window windowSize = GetWindowSize(window);
    ui_panels.window_w = (float)windowSize.w;
    ui_panels.window_h = (float)windowSize.h;
    panels.menu.panel = {0.0f, 0.0f, (float)windowSize.w, 25.0f};
    panels.top = {0.0f, panels.menu.panel.h, (float)windowSize.w, 75.0f};

    float topButtonHeight = Cap(panels.top.panel.h, 50.0f);
    btn_y = panels.top.panel.y + (panels.top.panel.h - topButtonHeight) / 2.0f;
    for (int i = 0; i < panels.top.buttons.size(); ++i) {
      float btn_x = panels.top.panel.x + ((float)i * topButtonHeight) + (((panels.top.panel.h - topButtonHeight) / 2.0f) * ((float)i + 1.0f)) ;
      panels.top.buttons[i] = {btn_x, btn_y, topButtonHeight, topButtonHeight};
    }

    panels.bottom = {0.0f, (float)windowSize.h - ui_panels.bottomHeight, (float)windowSize.w, ui_panels.bottomHeight};
    panels.center = {0.0f, panels.menu.panel.h + panels.top.panel.h, (float)windowSize.w, (float)windowSize.h - (panels.menu.panel.h + panels.top.panel.h + panels.bottom.h)};

    //left
    panels.mainPanel.left.panel = {0.0f, panels.center.y, ui_panels.leftPanelWidth, panels.center.h};
    panels.mainPanel.left.search = {panels.mainPanel.left.panel.x, panels.mainPanel.left.panel.y, ui_panels.leftPanelWidth, ui_panels.filterHeight};
    panels.mainPanel.left.body = {panels.mainPanel.left.panel.x, panels.mainPanel.left.panel.y + ui_panels.filterHeight, ui_panels.leftPanelWidth - ui_panels.scrollWidth, panels.center.h - ui_panels.filterHeight};
    //number of elements (it might cut off with integer)
    ui_panels.numElement = (int)(panels.mainPanel.left.body.h / (ui_panels.h + ui_panels.spacing)) + 1;
    panels.mainPanel.left.scroll.panel = {panels.mainPanel.left.panel.x + panels.mainPanel.left.panel.w - ui_panels.scrollWidth, panels.mainPanel.left.panel.y + ui_panels.filterHeight, ui_panels.scrollWidth, panels.mainPanel.left.panel.h - ui_panels.filterHeight};
    panels.mainPanel.left.scroll.bar = {panels.mainPanel.left.panel.x + panels.mainPanel.left.panel.w - ui_panels.scrollWidth, panels.mainPanel.left.panel.y + ui_panels.filterHeight + ui_panels.scrollBarLeftY, ui_panels.scrollWidth, ui_panels.scrollBarLeftHeight};
    panels.mainPanel.left.scroll.elementHeight = 40.0f;
    panels.mainPanel.left.scroll.elementSpacing = 5.0f;


    //right
    panels.mainPanel.right.panel = {panels.center.w - ui_panels.rightPanelWidth, panels.center.y, ui_panels.rightPanelWidth, panels.center.h};
    panels.mainPanel.right.body = {panels.mainPanel.right.panel.x, panels.mainPanel.right.panel.y, ui_panels.rightPanelWidth - ui_panels.scrollWidth, panels.center.h};
    panels.mainPanel.right.scroll.panel = {panels.mainPanel.right.panel.x + panels.mainPanel.right.panel.w - ui_panels.scrollWidth, panels.mainPanel.right.panel.y, ui_panels.scrollWidth, panels.mainPanel.right.panel.h};
    panels.mainPanel.right.scroll.bar = {panels.mainPanel.right.panel.x + panels.mainPanel.right.panel.w - ui_panels.scrollWidth, panels.mainPanel.right.panel.y + ui_panels.scrollBarRightY, ui_panels.scrollWidth, ui_panels.scrollBarRightHeight};
    panels.mainPanel.right.scroll.elementHeight = 25.0f;
    panels.mainPanel.right.scroll.elementSpacing =  5.0f;


    //center
    panels.mainPanel.center.panel = {panels.mainPanel.left.panel.w, panels.center.y, panels.center.w - (panels.mainPanel.left.panel.w + panels.mainPanel.right.panel.w), panels.center.h};

    panels.mainPanel.center.buttonBar = {panels.mainPanel.center.panel.x + ui_panels.expanderWidth, panels.mainPanel.center.panel.y, panels.mainPanel.center.panel.w - (ui_panels.expanderWidth * 2.0f), ui_panels.buttonBarHeight};
    float EditButtonHeight = Cap(panels.mainPanel.center.buttonBar.panel.h, 40.0f);
    btn_y = panels.mainPanel.center.buttonBar.panel.y + (panels.mainPanel.center.buttonBar.panel.h - EditButtonHeight) / 2.0f;
    for (int i = 0; i < panels.mainPanel.center.buttonBar.buttons.size(); ++i) {
      float btn_x = panels.mainPanel.center.buttonBar.panel.x + ((float)i * EditButtonHeight) + (((panels.mainPanel.center.buttonBar.panel.h - EditButtonHeight) / 2.0f) * ((float)i + 1.0f)) ;
      panels.mainPanel.center.buttonBar.buttons[i] = {btn_x, btn_y, EditButtonHeight, EditButtonHeight};
    }

    panels.mainPanel.center.expanderLeft = {panels.mainPanel.center.panel.x, panels.mainPanel.center.panel.y, ui_panels.expanderWidth, panels.mainPanel.center.panel.h};
    panels.mainPanel.center.expanderRight = {(panels.mainPanel.center.panel.x + panels.mainPanel.center.panel.w) - ui_panels.expanderWidth, panels.mainPanel.center.panel.y, ui_panels.expanderWidth, panels.mainPanel.center.panel.h};

    panels.mainPanel.center.shapes.panel = {(panels.mainPanel.center.panel.x + panels.mainPanel.center.panel.w) - ui_panels.shapeListWidth, panels.mainPanel.center.panel.y + ui_panels.buttonBarHeight, ui_panels.shapeListWidth, panels.mainPanel.center.panel.h - ui_panels.buttonBarHeight};
    panels.mainPanel.center.shapes.expanderLeft = {panels.mainPanel.center.shapes.panel.x, panels.mainPanel.center.panel.y + ui_panels.buttonBarHeight, ui_panels.expanderWidth, panels.mainPanel.center.panel.h - ui_panels.buttonBarHeight};
    panels.mainPanel.center.shapes.body = { + panels.mainPanel.center.shapes.panel.x + panels.mainPanel.center.shapes.expanderLeft.w, panels.mainPanel.center.panel.y + ui_panels.buttonBarHeight, panels.mainPanel.center.shapes.panel.w - ui_panels.scrollWidth - ui_panels.scrollWidth, panels.mainPanel.center.panel.h - ui_panels.buttonBarHeight};
    panels.mainPanel.center.shapes.scroll.panel = { panels.mainPanel.center.shapes.body.x + panels.mainPanel.center.shapes.body.w, panels.mainPanel.center.panel.y + ui_panels.buttonBarHeight, ui_panels.scrollWidth, panels.mainPanel.center.panel.h - ui_panels.buttonBarHeight};
    panels.mainPanel.center.shapes.scroll.bar = {panels.mainPanel.center.shapes.scroll.panel.x, panels.mainPanel.center.shapes.scroll.panel.y + ui_panels.scrollBarFixturesY, ui_panels.scrollWidth, ui_panels.scrollBarFixturesHeight};
    panels.mainPanel.center.shapes.scroll.elementHeight = 25.0f;
    panels.mainPanel.center.shapes.scroll.elementSpacing = 2.0f;

    panels.mainPanel.center.image = {panels.mainPanel.center.panel.x + ui_panels.expanderWidth + ui_panels.space, panels.mainPanel.center.panel.y + ui_panels.space + ui_panels.buttonBarHeight, panels.mainPanel.center.panel.w - panels.mainPanel.center.shapes.panel.w - ((ui_panels.expanderWidth * 2.0f)), panels.mainPanel.center.panel.h - (ui_panels.space * 2.0f) - ui_panels.buttonBarHeight};
    return panels;
  }

  void Show_Overlay(const Context &context) {
    SDL_SetRenderDrawColor(context.renderer, 0,0,0, 75);
    auto w = GetWindowSize(context.window);
    SDL_FRect rect = {0.0f, 0.0f, (float)w.w, (float)w.h };
    SDL_RenderFillRectF(context.renderer, &rect);
    SDL_RenderPresent(context.renderer);
  }

  void Wait(bool &loaded) {
    SDL_Event event;
    while (!loaded) {
      SDL_Delay(25);
      while (SDL_PollEvent(&event)) {
        SDL_Delay(25);
        if (event.type == SDL_QUIT) {
          return;
        }
      }
    }
  }

  SDL_Texture* Load_Texture(const Context &context, const std::string &filePath) {
    return IMG_LoadTexture(context.renderer, filePath.c_str());
  }

  void Import_Image(bool &loaded, std::string &fileName) {
    nfdchar_t *outPath;
    nfdresult_t result = NFD_OpenDialog("png,jpg", nullptr, &outPath);

    loaded = true;
    if ( result == NFD_CANCEL ||  result == NFD_ERROR ) {
      free(outPath);
    }

    else if ( result == NFD_OKAY ) {
      fileName = outPath;
      free(outPath);
    }
  }

  Image_Import Load_Image(const Context &context) {
    bool loaded = false;
    std::string fileName;

    auto thread_func = [&loaded, &fileName] () mutable {
      Import_Image(loaded, fileName);
    };

    Show_Overlay(context);

    std::thread LOAD_THREAD(thread_func);
    SDL_Event event;
    Wait(loaded);
    LOAD_THREAD.join();

    SDL_Texture *texture = IMG_LoadTexture(context.renderer, fileName.c_str());
    return {texture, fileName};
  }

  void Import_Images(bool &loaded, std::vector<Graphics::Image_Import> &imageImport) {
    Graphics::Image_Import image{};

    nfdpathset_t pathSet;
    nfdresult_t result = NFD_OpenDialogMultiple("png,jpg", nullptr, &pathSet);

    if ( result == NFD_CANCEL ||  result == NFD_ERROR ) {
      NFD_PathSet_Free(&pathSet);
    }
    else if ( result == NFD_OKAY ) {
      size_t i;
      nfdchar_t *outPath;
      for ( i = 0; i < NFD_PathSet_GetCount(&pathSet); ++i ) {
        outPath = NFD_PathSet_GetPath(&pathSet, i);
        image.fileName = outPath;
        imageImport.emplace_back(image);
      }
      NFD_PathSet_Free(&pathSet);
    }
    loaded = true;
  }

  std::vector<Graphics::Image_Import> Load_Images(const Context &context) {
    std::vector<Graphics::Image_Import> imageImport{};

    bool loaded = false;
    Show_Overlay(context);

    auto thread_func = [&loaded, &imageImport] () mutable {
      Import_Images(loaded, imageImport);
    };

    std::thread LOAD_THREAD(thread_func);
    Wait(loaded);
    LOAD_THREAD.join();

    for (auto &image : imageImport) {
      image.texture = IMG_LoadTexture(context.renderer, image.fileName.c_str());
    }
    return imageImport;
  }

  SDL_Texture* Render_Circle(SDL_Renderer *renderer, const float &x, const float &y, const float &r, const SDL_Color &shapeFill, const SDL_Color &edgeColor) {
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, int(r * 2), int(r * 2));
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    //check where the screen is relative to center point
    //only render a pie piece instead of whole thing
    //this way we can rerender on zoom

    SDL_FRect rect = {0.0f, 0.0f, r * 2.0f, r * 2.0f};
    SDL_SetRenderDrawColor(renderer, 120, 0, 0, 120);
    SDL_RenderFillRectF(renderer, &rect);
    aaFilledPieRGBA(renderer, r, r, r, r, 0.0f, 360.0f, 0, shapeFill.r, shapeFill.g, shapeFill.b, shapeFill.a);
    SDL_SetRenderTarget(renderer, nullptr);
    return texture;
  }

  SDL_Texture* Render_Polygon_AABB(SDL_Renderer *renderer, std::vector<double> &xPolygonPoints, std::vector<double> &yPolygonPoints, const int &w, const int &h, const SDL_Color &shapeFill) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, texture);
    double lowestX =  std::numeric_limits<double>::max();
    for (const auto &num: xPolygonPoints) {
      if (num < lowestX)
        lowestX = num;
    }
    for (auto &num: xPolygonPoints) {
      num -= lowestX;
    }

    double lowestY =  std::numeric_limits<double>::max();
    for (const auto &num: yPolygonPoints) {
      if (num < lowestY)
        lowestY = num;
    }
    for (auto &num: yPolygonPoints) {
      num -= lowestY;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    SDL_RenderClear(renderer);
    aaFilledPolygonRGBA(renderer, xPolygonPoints.data(), yPolygonPoints.data(), (int) yPolygonPoints.size(), shapeFill.r, shapeFill.g, shapeFill.b, shapeFill.a);
    SDL_SetRenderTarget(renderer, nullptr);
    return texture;
  }
}