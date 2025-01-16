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
#include <vector>
#include <ranges>
#include <iostream>
#include <string>
#include "../../UI/scroll_bar.h"

namespace Center::Left {

  void Render(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.body);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.body);

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.search);
    if (app.filterImages && app.interface.left.filteredIndexes.empty() && !app.filterText.empty())
      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
    else
      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.search);

    auto caretRect = Text::Render(app.context.renderer, app.context.font, app.filterText.c_str(), app.panel.mainPanel.left.search.x + app.uiPanels.spacing, app.panel.mainPanel.left.search.y);

    if (app.filterImages) { //render caret
      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
      SDL_RenderDrawLineF(app.context.renderer, (float) caretRect.x + (float) caretRect.w, (float) caretRect.y, (float) caretRect.x + (float) caretRect.w, (float) caretRect.y + (float) caretRect.h);
    }

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.panel);

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.bar);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.left.scroll.bar);

    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;


    //Instead of starting at zero, start at where the scroll bar is relative to the length  of the list
    // (the number of element that currently fit on the rendering frame) and end at where they cut off


    // maxSize - numElementsShown if > 0 then the bar should exist
    // and be the height of     (numElementsShown / max) * bar.panel.h
    // so it is proportional to the size of the list

    if (!app.interface.left.filteredIndexes.empty()) {
      Scroll_Bar::Set_Bar_Size(app.uiPanels.numElement, app.interface.left.filteredIndexes.size(), app.panel.mainPanel.left.scroll.panel.h, app.uiPanels.scrollBarLeftHeight);
      auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
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
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
          SDL_RenderFillRectF(app.context.renderer, &rect);
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
          SDL_RenderDrawRectF(app.context.renderer, &rect);
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
      Scroll_Bar::Set_Bar_Size(app.uiPanels.numElement, app.interface.left.images.size(), app.panel.mainPanel.left.scroll.panel.h, app.uiPanels.scrollBarLeftHeight);
      auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
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
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
          SDL_RenderFillRectF(app.context.renderer, &rect);
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
          SDL_RenderDrawRectF(app.context.renderer, &rect);
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

    auto cursor = Mouse::Cursor();
    for (int i = 0; i < app.interface.left.images.size(); ++i) {
      SDL_FRect dRect = {x + app.panel.mainPanel.left.scroll.elementSpacing, y + app.panel.mainPanel.left.scroll.elementSpacing, app.panel.mainPanel.left.body.w - (app.panel.mainPanel.left.scroll.elementSpacing * 3.0f), app.panel.mainPanel.left.scroll.elementHeight};
      if (Rect_Intersect(cursor, dRect)) {
        return true;
      }
      y += app.panel.mainPanel.left.scroll.elementHeight + app.panel.mainPanel.left.scroll.elementSpacing;
    }
    return false;
  };

  Data::Center Select_Image(App::App &app) {
    float x = app.panel.mainPanel.left.body.x;
    float y = app.panel.mainPanel.left.body.y;

    auto cursor = Mouse::Cursor();
    if (!app.interface.left.filteredIndexes.empty()) {
      auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
                                      app.panel.mainPanel.left.scroll.panel.h,
                                      app.uiPanels.scrollBarLeftY,
                                      app.uiPanels.scrollBarLeftHeight,
                                      app.interface.left.filteredIndexes.size(),
                                      app.uiPanels.numElement);
      for (int i = index.min; i < index.max; ++i) {
        const int &index = app.interface.left.filteredIndexes[i];

        SDL_FRect dRect = {x + app.uiPanels.spacing, y + app.uiPanels.spacing, app.panel.mainPanel.left.body.w - (app.uiPanels.spacing * 3.0f), app.uiPanels.h};
        if (Rect_Intersect(cursor, dRect)) {
          app.imageIndex = index;
          return app.interface.left.images[index];
        }
        y += app.uiPanels.h + app.uiPanels.spacing;
      }
    }
    else {
      auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.left.scroll.bar.x,
                                      app.panel.mainPanel.left.scroll.panel.h,
                                      app.uiPanels.scrollBarLeftY,
                                      app.uiPanels.scrollBarLeftHeight,
                                      app.interface.left.images.size(),
                                      app.uiPanels.numElement);
      for (int i = index.min; i < index.max; ++i) {
        SDL_FRect dRect = {x + app.uiPanels.spacing, y + app.uiPanels.spacing, app.panel.mainPanel.left.body.w - (app.uiPanels.spacing * 3.0f), app.uiPanels.h};
        if (Rect_Intersect(cursor, dRect)) {
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
      app.selectedShape.shape = Shape::SIZE;
      app.selectedVertex.shape = Shape::SIZE;

      //clear and repopulate shape list
      Shape::Shape_List shapeList;
      for (int i = 0; i < Shape::SIZE; ++i) {
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
    Scroll_Bar::Scroll(app,
                       app.panel.mainPanel.left.scroll,
                       app.uiPanels.scrollBarLeftY,
                       app.uiPanels.scrollBarFixturesHeight,
                       app.interface.left.images.size(),
                       scroll,
                       app.panel.mainPanel.left.body.w
    );

    return true;
  }
}
