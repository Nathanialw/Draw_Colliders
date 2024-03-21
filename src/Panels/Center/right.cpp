//
// Created by nathanial on 3/10/24.
//
#include "right.h"

namespace Center::Right {
  void Render(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.right.body);
    SDL_SetRenderDrawColor(app.context.renderer, 25, 25, 25, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.right.body);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.right.scroll.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 25, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.right.scroll.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.right.scroll.bar);
    SDL_SetRenderDrawColor(app.context.renderer, 152, 25, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.right.scroll.bar);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  };


  bool Scroll(App::App &app, const Sint32 &scroll) {
    int maxElementsToDisplay = 0;
    int numElements = 0;

    if (scroll < 0) {
      app.uiPanels.scrollBarRightY += 10.0f;
      if (app.uiPanels.scrollBarRightY > app.panel.mainPanel.right.scroll.panel.h - app.panel.mainPanel.right.scroll.bar.h)
        app.uiPanels.scrollBarRightY = app.panel.mainPanel.right.scroll.panel.h - app.panel.mainPanel.right.scroll.bar.h;
    }
    else {
      app.uiPanels.scrollBarRightY -= 10.0f;
      if (app.uiPanels.scrollBarRightY < 0.0f) app.uiPanels.scrollBarRightY = 0.0f;
    }
    App::Set_Bar_Size(maxElementsToDisplay, numElements, app.panel.mainPanel.center.shapes.scroll.panel.h, app.uiPanels.scrollBarFixturesHeight);
    app.panel = Graphics::Set_Panels(app.context.window, app.uiPanels);
    App::Set_Textures(app);
    return true;
  }
}