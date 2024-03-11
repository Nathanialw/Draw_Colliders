//
// Created by nathanial on 3/10/24.
//
#include "top.h"

namespace Top {
  bool Top_Panel() {
    return false;
  };

  void Render_Button_Bar(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.top.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.top.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    for (const auto &btn: app.panel.top.buttons) {
      SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      SDL_RenderFillRectF(app.context.renderer, &btn);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
      SDL_RenderDrawRectF(app.context.renderer, &btn);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    }
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.buttonBar);
  };
}