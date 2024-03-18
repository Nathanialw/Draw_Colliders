//
// Created by nathanial on 3/10/24.
//
#include "../../App/core.h"
#include "../../Input/mouse.h"

#include "SDL2/SDL.h"
#include "../../Utils/utils.h"
#include "../../Graphics/text.h"

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

    Text::Render(app.context.renderer, app.context.font, app.filterText.c_str(), app.panel.mainPanel.left.search.x, app.panel.mainPanel.left.search.y);

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
    for (int i = 0; i < app.interface.left.images.size(); ++i) {
      SDL_FRect dRect = {x + spacing, y + spacing, w - (spacing * 2.0f), h};
      if (app.interface.left.selected == i)      {
        SDL_FRect rect = {
            dRect.x + spacing,
            dRect.y,
            app.panel.mainPanel.left.body.w - (spacing * 2.0f),
            dRect.h,
        };
        SDL_SetRenderDrawColor(app.context.renderer, 100, 100, 200, 255);
        SDL_RenderFillRectF(app.context.renderer, &rect);
        SDL_SetRenderDrawColor(app.context.renderer, 155, 155, 155, 255);
        SDL_RenderDrawRectF(app.context.renderer, &rect);
        SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      }
      SDL_RenderCopyF(app.context.renderer, app.interface.left.images[i].texture.texture, nullptr, &dRect);
      SDL_Rect rect = {
          (int)dRect.x + (int)dRect.w + (int)spacing,
          (int)dRect.y,
          (int)app.panel.mainPanel.left.body.w - (int)dRect.w - (int)(spacing * 3.0f),
          (int)dRect.h,
      };
      Text::Render(app.context.renderer, app.context.font, app.interface.left.imageNameStr[i].c_str(), rect);
      y += h + spacing;
    }
  }

  Data::Center Select_Image(App::App &app) {
    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;
    float w = 40.0f;
    float h = 40.0f;
    float spacing = 5.0f;

    auto cursor = Mouse::Cursor();
    for (int i = 0; i < app.interface.left.images.size(); ++i) {

      SDL_FRect dRect = {x + spacing, y + spacing, app.panel.mainPanel.left.body.w - (spacing * 3.0f), h};
      if (SDL_HasIntersectionF(&cursor, &dRect)) {
        //mouseover highlighting
//        app.interface.left.selected = i;
        app.imageIndex = i;
        return app.interface.left.images[i];
      }
      y += h + spacing;
    }
    return {nullptr};
  };

  bool Set_Image(App::App &app) {
    auto image = Select_Image(app);
    if (image.texture.texture) {
      if (app.interface.center.texture.texture) {
        Data::Center currentImage = app.interface.center;
        if (currentImage.index == image.index)
          return false;
        //needs to go back where it was
        app.interface.left.selected = image.index;
        app.interface.left.images[currentImage.index] = currentImage;
      }
      app.interface.center = image;
      app.selectedShape.shape = Graphics::SIZE;
      app.selectedVertex.shape = Graphics::SIZE;
      return true;
    }
    return false;
  }

  bool Filter_Images(App::App &app) {
    //set flag to redirect keyboard input to text input
    app.filterImages = true;
    app.filterText = "";
    return true;
  }

  bool Scroll(App::App &app) {
    //scroll down the list of images
    return true;
  }
}
