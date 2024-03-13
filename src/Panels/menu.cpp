//
// Created by nathanial on 3/10/24.
//
#include "array"
#include "SDL2/SDL.h"
#include "menu.h"
#include "../Graphics/text.h"

namespace Menu {

  struct Button {
    SDL_Texture* texture{};
    SDL_FRect rect{};
  };

  std::array<std::string, Graphics::numMenuButtons> text = {"File", "Edit", "Options", "View", "Window", "Help"};
  std::array<Button, Graphics::numMenuButtons> menu = {};

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
    float spacing = 5.0f;
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    for (const auto &button: text) {
      SDL_FRect dRect = {x + spacing, y, w, h};
      //render to text, not every frame
      auto rect = Text::Render(app.context.renderer, app.context.font, button.c_str(), dRect.x, dRect.y);
      rect.w += (int)spacing * 2;
      rect.x -= (int)spacing;
      SDL_RenderDrawRect(app.context.renderer, &rect);
      x += (float)rect.w;
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  bool Top_Menu() {
    return false;
  };
}