//
// Created by nathanial on 3/9/24.
//
#include "graphics.h"
#include "render.h"
#include "../Input/mouse.h"

#include "../Panels/Center/left.h"
#include "../Panels/Center/right.h"
#include "../Panels/Center/center.h"
#include "../Panels/top.h"
#include "../Panels/menu.h"
#include "../Panels/bottom.h"

namespace Render {
  using namespace Graphics;

  void Render_Panel (const App::App &app, const SDL_FRect &rect, const int r, const int g, const int b) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &rect);
    SDL_SetRenderDrawColor(app.context.renderer, r, g, b, 255);
    SDL_RenderDrawRectF(app.context.renderer, &rect);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Copy(App::App &app) {
//    Render_Panel(app, app.panel.mainPanel.center.panel, 50, 50, 100);
    Center::Center::Render(app);
    Center::Right::Render(app);
    Center::Left::Render(app);

    Render_Panel(app, app.panel.top.panel, 50, 155, 100);
    ::Top::Render_Button_Bar(app);
    ::Menu::Render(app);
    Render_Panel(app, app.panel.bottom, 25, 50, 100);
  }

  void Present(App::App &app) {
    SDL_RenderClear(app.context.renderer);
    Copy(app);
    SDL_RenderPresent(app.context.renderer);
  };

}