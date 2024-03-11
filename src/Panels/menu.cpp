//
// Created by nathanial on 3/10/24.
//
#include "array"
#include "SDL2/SDL.h"
#include "menu.h"

namespace Menu {
  struct Button {
    SDL_Texture* texture;
    SDL_FRect rect;
  };

  std::array<Button, 4> menu = {};

  void Render(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.menu.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.menu.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    float x = app.panel.menu.panel.x;
    float y = app.panel.menu.panel.y;
    float w = 50.0f;
    float h = app.panel.menu.panel.h;
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    for (const auto &button: app.panel.menu.buttons) {
      SDL_FRect dRect = {x, y, w, h};
      SDL_RenderDrawRectF(app.context.renderer, &dRect);
//      SDL_RenderCopyF(app.context.renderer, button.texture, nullptr, &dRect);
      x += w;
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  bool Top_Menu() {
    return false;
  };
}