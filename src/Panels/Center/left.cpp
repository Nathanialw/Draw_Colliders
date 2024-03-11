//
// Created by nathanial on 3/10/24.
//
#include "../../App/core.h"
#include "../../Input/mouse.h"

#include "SDL2/SDL.h"

namespace Center::Left {

  void Render(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.body);
    SDL_SetRenderDrawColor(app.context.renderer, 25, 25, 25, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.body);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.search);
    SDL_SetRenderDrawColor(app.context.renderer, 125, 125, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.search);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 25, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.bar);
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.bar);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;
    float w = 40.0f;
    float h = 40.0f;
    float spacing = 5.0f;
    for (const auto &image: app.appData.left.images) {
      SDL_FRect dRect = {x + spacing, y + spacing, w - (spacing * 2.0f), h - spacing};
      SDL_RenderCopyF(app.context.renderer, image.texture, nullptr, &dRect);
      y += h + spacing;
    }
  }

  Graphics::Image Select_Image(App::App &app) {
    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;
    float w = 40.0f;
    float h = 40.0f;
    float spacing = 5.0f;

    auto cursor = Mouse::Cursor();
    for (const auto &image: app.appData.left.images) {
      SDL_FRect dRect = {x + spacing, y + spacing, w - (spacing * 2.0f), h - spacing};
      if (SDL_HasIntersectionF(&cursor, &dRect))
        return image;
      y += h + spacing;
    }

    return {nullptr};
  };

  bool Scroll(App::App &app) {
    //scroll down the list of images
    return true;
  }
}
