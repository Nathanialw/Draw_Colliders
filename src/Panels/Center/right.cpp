//
// Created by nathanial on 3/10/24.
//
#include "right.h"
#include "../../UI/scroll_bar.h"
#include "../../Graphics/text.h"
#include "../../Input/mouse.h"

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

    int maxElementsToDisplay = (int)(app.panel.mainPanel.right.body.h / (app.panel.mainPanel.right.scroll.elementHeight + app.panel.mainPanel.right.scroll.elementSpacing)) + 1;

    Scroll_Bar::Set_Bar_Size(maxElementsToDisplay, app.interface.right.option.size(), app.panel.mainPanel.right.scroll.panel.h, app.uiPanels.scrollBarRightHeight);
    auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.right.scroll.bar.x,
                                           app.panel.mainPanel.right.scroll.panel.h,
                                           app.uiPanels.scrollBarRightY,
                                           app.uiPanels.scrollBarRightHeight,
                                           app.interface.right.option.size(),
                                           maxElementsToDisplay);

    float x = app.panel.mainPanel.right.body.x;
    float y = app.panel.mainPanel.right.body.y;

    for (int i = index.min; i < index.max; ++i) {
      if (app.interface.right.optionInput[i].type == Data::NONE) {
        y += app.panel.mainPanel.right.scroll.elementHeight + app.panel.mainPanel.right.scroll.elementSpacing;
        continue;
      }

      SDL_Rect rect = {
          (int) (x + app.panel.mainPanel.right.scroll.elementSpacing),
          (int) (y + app.panel.mainPanel.right.scroll.elementSpacing),
          (int) app.panel.mainPanel.right.body.w,
          (int) app.panel.mainPanel.right.scroll.elementHeight
      };

      SDL_FRect fRect = {(x + rect.w  - (app.panel.mainPanel.right.scroll.elementSpacing * 2.0f + app.uiPanels.scrollWidth)), (y + app.panel.mainPanel.right.scroll.elementSpacing), (float)rect.h, (float)rect.h};

      if (app.interface.right.optionInput[i].type == Data::CHECKBOX) {
        SDL_SetRenderDrawColor(app.context.renderer, 200, 200, 200, 255);
        SDL_RenderFillRectF(app.context.renderer, &fRect);
        (app.interface.right.option[i].isChecked) ? SDL_RenderCopyF(app.context.renderer, app.texture.checkedBox, nullptr, &fRect) : SDL_RenderCopyF(app.context.renderer, app.texture.uncheckedBox, nullptr, &fRect);
        SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
      }

      Text::Render(app.context.renderer, app.context.font, app.interface.right.optionName[i].c_str(), rect);
      y += app.panel.mainPanel.right.scroll.elementHeight + app.panel.mainPanel.right.scroll.elementSpacing;
    }
  }

  void Select_Option(App::App &app) {
    int maxElementsToDisplay = (int)(app.panel.mainPanel.right.body.h / (app.panel.mainPanel.right.scroll.elementHeight + app.panel.mainPanel.right.scroll.elementSpacing)) + 1;

    Scroll_Bar::Set_Bar_Size(maxElementsToDisplay, app.interface.right.option.size(), app.panel.mainPanel.right.scroll.panel.h, app.uiPanels.scrollBarRightHeight);
    auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.right.scroll.bar.x,
                                           app.panel.mainPanel.right.scroll.panel.h,
                                           app.uiPanels.scrollBarRightY,
                                           app.uiPanels.scrollBarRightHeight,
                                           app.interface.right.option.size(),
                                           maxElementsToDisplay);

    float x = app.panel.mainPanel.right.body.x;
    float y = app.panel.mainPanel.right.body.y;
    for (int i = index.min; i < index.max; ++i) {
      if (app.interface.right.optionInput[i].type == Data::NONE) {
        y += app.panel.mainPanel.right.scroll.elementHeight + app.panel.mainPanel.right.scroll.elementSpacing;
        continue;
      }
      //if mouse is inside option
        SDL_Rect rect = {
            (int) (x + app.panel.mainPanel.right.scroll.elementSpacing),
            (int) (y + app.panel.mainPanel.right.scroll.elementSpacing),
            (int) app.panel.mainPanel.right.body.w,
            (int) app.panel.mainPanel.right.scroll.elementHeight
        };

        SDL_FRect fRect = {(x + rect.w - (app.panel.mainPanel.right.scroll.elementSpacing * 2.0f + app.uiPanels.scrollWidth)), (y + app.panel.mainPanel.right.scroll.elementSpacing), (float) rect.h, (float) rect.h};
      if (Point_FRect_Intersect(Mouse::Cursor_Point(), fRect)) {
        if (app.interface.right.optionInput[i].type == Data::CHECKBOX) {
          app.interface.right.option[i].isChecked = !app.interface.right.option[i].isChecked;
//          app.interface.left.images[app.interface.center.index].options[i].isChecked = !app.interface.left.images[app.interface.center.index].options[i].isChecked;
        }
      }
      y += app.panel.mainPanel.right.scroll.elementHeight + app.panel.mainPanel.right.scroll.elementSpacing;
    }
  }

  bool Scroll(App::App &app, const Sint32 &scroll) {

    Scroll_Bar::Scroll(app,
                       app.panel.mainPanel.right.scroll,
                       app.uiPanels.scrollBarRightY,
                       app.uiPanels.scrollBarFixturesHeight,
                       app.interface.right.option.size(),
                       scroll,
                       app.panel.mainPanel.right.body.w
        );

    return true;
  }
}