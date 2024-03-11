//
// Created by nathanial on 3/10/24.
//
#include "iostream"

#include "SDL2/SDL.h"
#include "left.h"
#include "center.h"

namespace Center::Center {
  bool move = false;
  int appMax = 10;
  int imageMoveMax = 20;

  SDL_Point initialPosition;
  SDL_Point offset = {0, 0};

  bool Set_Scale(App::App &app, Sint32 direction) {
    if (direction < 0)
      app.appData.center.texture.scale *= 0.9;
    if (direction > 0)
      app.appData.center.texture.scale /= 0.9;
    return true;
  }

  bool isMoving() {
    return move;
  }

  bool Move(App::App &app) {
    app.max = imageMoveMax;
    SDL_GetMouseState(&initialPosition.x, &initialPosition.y);
    move = true;
    return true;
  }

  bool Set(App::App &app) {
    app.max = appMax;
    app.appData.center.texture.offset.x = app.appData.center.texture.offset.x + (float)offset.x;
    app.appData.center.texture.offset.y = app.appData.center.texture.offset.y + (float)offset.y;
    offset = {0, 0};
    move = false;
    return true;
  }

  bool Update_Image() {
    if (move) {
      int x;
      int y;
      SDL_GetMouseState(&x, &y);
      offset = {initialPosition.x - x, initialPosition.y - y};
      return true;
    }
    return false;
  }

  bool Set_Edit_Image(App::App &app) {
    Graphics::Image image = Left::Select_Image(app);
    if (image.texture)
      app.appData.center.texture = image;
    return true;
  }

  void Render_Image(App::App &app) {
    if (app.appData.center.texture.texture) {
      SDL_Point size;
      SDL_QueryTexture(app.appData.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
      float w = (float) size.x * app.appData.center.texture.scale;
      float h = (float) size.y * app.appData.center.texture.scale;

      SDL_FRect rect = {
          app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w) / 2.0f),
          app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h) / 2.0f),
          w,
          h,
      };
      rect.x -= ((float) offset.x + (float) app.appData.center.texture.offset.x);
      rect.y -= ((float) offset.y + (float) app.appData.center.texture.offset.y);
      SDL_RenderCopyF(app.context.renderer, app.appData.center.texture.texture, nullptr, &rect);
    }
  }

  void Render_left_Scroll(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.expanderLeft);
    SDL_SetRenderDrawColor(app.context.renderer, 155, 155, 155, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.expanderLeft);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderLeft);
  };

  void Render_Right_Scroll(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.expanderRight);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 55, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.expanderRight);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderRight);
  };

  void Render_Button_Bar(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.buttonBar.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.buttonBar.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    for (const auto &btn: app.panel.mainPanel.center.buttonBar.buttons) {
      SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      SDL_RenderFillRectF(app.context.renderer, &btn);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
      SDL_RenderDrawRectF(app.context.renderer, &btn);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    }
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.buttonBar);
  };

  void Render(App::App &app) {
    Render_Image(app);
    Render_left_Scroll(app);
    Render_Right_Scroll(app);
    Render_Button_Bar(app);
  }
}