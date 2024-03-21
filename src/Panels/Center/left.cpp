//
// Created by nathanial on 3/10/24.
//
#include <algorithm>
#include "../../App/core.h"
#include "../../Input/mouse.h"

#include "SDL2/SDL.h"
#include "../../Utils/utils.h"
#include "../../Graphics/text.h"

#include <iterator>
#include <unistd.h>
#include <vector>
#include <ranges>
#include <iostream>
#include <string>

namespace Center::Left {

  void Render(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.body);
    SDL_SetRenderDrawColor(app.context.renderer, 25, 25, 25, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.body);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.search);
    if (app.filterImages && app.interface.left.filteredIndexes.empty() && !app.filterText.empty())
      SDL_SetRenderDrawColor(app.context.renderer, 255, 125, 125, 255);
    else
      SDL_SetRenderDrawColor(app.context.renderer, 125, 125, 125, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.search);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    auto caretRect = Text::Render(app.context.renderer, app.context.font, app.filterText.c_str(), app.panel.mainPanel.left.search.x, app.panel.mainPanel.left.search.y);

    if (app.filterImages) { //render caret
      SDL_SetRenderDrawColor(app.context.renderer, 255, 255, 255, 255);
      SDL_RenderDrawLineF(app.context.renderer, (float) caretRect.x + (float) caretRect.w, (float) caretRect.y, (float) caretRect.x + (float) caretRect.w, (float) caretRect.y + (float) caretRect.h);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    }

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


    //Instead of starting at zero, start at where the scroll bar is relative to the length  of the list
    // (the number of element that currently fit on the rendering frame) and end at where they cut off


    // maxSize - numElementsShown if > 0 then the bar should exist
    // and be the height of     (numElementsShown / max) * bar.panel.h
    // so it is proportional to the size of the list

    if (!app.interface.left.filteredIndexes.empty()) {
      App::Set_Bar_Size(app.uiPanels.numElement, app.interface.left.filteredIndexes.size(), app.panel.mainPanel.left.scroll.panel.h, app.uiPanels.scrollBarLeftHeight);
      auto index = App::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
                                      app.panel.mainPanel.left.scroll.panel.h,
                                      app.uiPanels.scrollBarLeftY,
                                      app.uiPanels.scrollBarLeftHeight,
                                      app.interface.left.filteredIndexes.size(),
                                      app.uiPanels.numElement);

      for (int i = index.min; i < index.max; ++i) {
        const int &filterIndex = app.interface.left.filteredIndexes[i];
        SDL_FRect dRect = {x + app.uiPanels.spacing, y + app.uiPanels.spacing, app.uiPanels.w - (app.uiPanels.spacing * 2.0f), app.uiPanels.h};
        if (app.interface.left.selected == filterIndex) {
          SDL_FRect rect = {
              dRect.x + app.uiPanels.spacing,
              dRect.y,
              app.panel.mainPanel.left.body.w - (app.uiPanels.spacing * 2.0f),
              dRect.h,
          };
          SDL_SetRenderDrawColor(app.context.renderer, 100, 100, 200, 255);
          SDL_RenderFillRectF(app.context.renderer, &rect);
          SDL_SetRenderDrawColor(app.context.renderer, 155, 155, 155, 255);
          SDL_RenderDrawRectF(app.context.renderer, &rect);
          SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
        }
        SDL_RenderCopyF(app.context.renderer, app.interface.left.images[filterIndex].texture.texture, nullptr, &dRect);
        SDL_Rect rect = {
            (int) dRect.x + (int) dRect.w + (int) app.uiPanels.spacing,
            (int) dRect.y,
            (int) app.panel.mainPanel.left.body.w - (int) dRect.w - (int) (app.uiPanels.spacing * 3.0f),
            (int) dRect.h,
        };
        Text::Render(app.context.renderer, app.context.font, app.interface.left.imageNameStr[filterIndex].c_str(), rect);
        y += app.uiPanels.h + app.uiPanels.spacing;
      }
    }

    else {
      App::Set_Bar_Size(app.uiPanels.numElement, app.interface.left.images.size(), app.panel.mainPanel.left.scroll.panel.h, app.uiPanels.scrollBarLeftHeight);
      auto index = App::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
                    app.panel.mainPanel.left.scroll.panel.h,
                    app.uiPanels.scrollBarLeftY,
                    app.uiPanels.scrollBarLeftHeight,
                    app.interface.left.images.size(),
                    app.uiPanels.numElement);

      for (int i = index.min; i < index.max; ++i) {
        SDL_FRect dRect = {x + app.uiPanels.spacing, y + app.uiPanels.spacing, app.uiPanels.w - (app.uiPanels.spacing * 2.0f), app.uiPanels.h};
        if (app.interface.left.selected == i) {
          SDL_FRect rect = {
              dRect.x + app.uiPanels.spacing,
              dRect.y,
              app.panel.mainPanel.left.body.w - (app.uiPanels.spacing * 2.0f),
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
            (int) dRect.x + (int) dRect.w + (int) app.uiPanels.spacing,
            (int) dRect.y,
            (int) app.panel.mainPanel.left.body.w - (int) dRect.w - (int) (app.uiPanels.spacing * 3.0f),
            (int) dRect.h,
        };
        Text::Render(app.context.renderer, app.context.font, app.interface.left.imageNameStr[i].c_str(), rect);
        y += app.uiPanels.h + app.uiPanels.spacing;
      }
    }
  }

  bool Hover_Image(App::App &app) {
    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;
    float w = 40.0f;
    float h = 40.0f;
    float spacing = 5.0f;

    auto cursor = Mouse::Cursor();
    for (int i = 0; i < app.interface.left.images.size(); ++i) {
      SDL_FRect dRect = {x + spacing, y + spacing, app.panel.mainPanel.left.body.w - (spacing * 3.0f), h};
      if (SDL_HasIntersectionF(&cursor, &dRect)) {
        return true;
      }
      y += h + spacing;
    }
    return false;
  };

  Data::Center Select_Image(App::App &app) {
    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;

    auto cursor = Mouse::Cursor();
    if (!app.interface.left.filteredIndexes.empty()) {
      auto index = App::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
                                      app.panel.mainPanel.left.scroll.panel.h,
                                      app.uiPanels.scrollBarLeftY,
                                      app.uiPanels.scrollBarLeftHeight,
                                      app.interface.left.filteredIndexes.size(),
                                      app.uiPanels.numElement);
      for (int i = index.min; i < index.max; ++i) {
        const int &index = app.interface.left.filteredIndexes[i];

        SDL_FRect dRect = {x + app.uiPanels.spacing, y + app.uiPanels.spacing, app.panel.mainPanel.left.body.w - (app.uiPanels.spacing * 3.0f), app.uiPanels.h};
        if (SDL_HasIntersectionF(&cursor, &dRect)) {
          app.imageIndex = index;
          return app.interface.left.images[index];
        }
        y += app.uiPanels.h + app.uiPanels.spacing;
      }
    }
    else {
      auto index = App::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
                                      app.panel.mainPanel.left.scroll.panel.h,
                                      app.uiPanels.scrollBarLeftY,
                                      app.uiPanels.scrollBarLeftHeight,
                                      app.interface.left.images.size(),
                                      app.uiPanels.numElement);
      for (int i = index.min; i < index.max; ++i) {
        SDL_FRect dRect = {x + app.uiPanels.spacing, y + app.uiPanels.spacing, app.panel.mainPanel.left.body.w - (app.uiPanels.spacing * 3.0f), app.uiPanels.h};
        if (SDL_HasIntersectionF(&cursor, &dRect)) {
          //mouseover highlighting
    //        app.interface.left.selected = i;
          app.imageIndex = i;
          return app.interface.left.images[i];
        }
        y += app.uiPanels.h + app.uiPanels.spacing;
      }
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

      //clear and repopulate shape list
      Data::Shape_List shapeList;
      for (int i = 0; i < Graphics::Shape::SIZE; ++i) {
        for (int j = 0; j < app.interface.center.shapes[i].size(); ++j) {
          shapeList.shapeList[i].push_back(std::to_string(j));
        }
      }
      app.interface.shapeList = shapeList;
      return true;
    }
    return false;
  }

  bool Filter_Images(App::App &app) {
    //set flag to redirect keyboard input to text input
    if (app.interface.left.filteredIndexes.empty()) {
      app.interface.left.filteredIndexes.clear();
      app.filterText = "";
    }
    app.filterImages = true;
    return true;
  }

  bool Scroll(App::App &app, const Sint32 &scroll) {
    if (scroll < 0) {
      app.uiPanels.scrollBarLeftY += 10.0f;
     if (app.uiPanels.scrollBarLeftY > app.panel.mainPanel.left.scroll.panel.h - app.panel.mainPanel.left.scroll.bar.h)
       app.uiPanels.scrollBarLeftY = app.panel.mainPanel.left.scroll.panel.h - app.panel.mainPanel.left.scroll.bar.h;
    }
    else {
      app.uiPanels.scrollBarLeftY -= 10.0f;
      if (app.uiPanels.scrollBarLeftY < 0.0f) app.uiPanels.scrollBarLeftY = 0.0f;
    }
    App::Set_Bar_Size(app.uiPanels.numElement, app.interface.left.images.size(), app.panel.mainPanel.left.scroll.panel.h, app.uiPanels.scrollBarLeftHeight);
    app.panel = Graphics::Set_Panels(app.context.window, app.uiPanels);
    App::Set_Textures(app);
    return true;
  }
}
