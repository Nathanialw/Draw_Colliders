//
// Created by nathanial on 3/23/24.
//

#include "scroll_bar.h"
#include "../Input/mouse.h"

namespace Scroll_Bar {


  Index Get_Min_Index(const float &scrollPanelX, const float &scrollH, const float &scrollBarY, const float &barH, const int &containerSize, const int &numElements) {
    float offset = (scrollPanelX + scrollBarY) - scrollPanelX;
    float remaining = (scrollH - barH);
    float pixelCount = remaining / ((float)(containerSize) - (float)numElements + 1.0f);

    int minIndex = (offset / pixelCount);
    if (minIndex < 0)
      minIndex = 0;

    int max = Get_Min(numElements, containerSize);
    auto maxIndex = (minIndex + max);
    if (maxIndex > containerSize)
      maxIndex = containerSize;

    return {minIndex, maxIndex};
  }

  //set body.w += bar.w
  //set bar.w -= bar.w and bar.x += bar.w
  bool Set_Bar_Size(const float &numElement, const float &num, const float &panelH, float &barH) {
    if ((numElement - num) < 0) {
      float percentSize = numElement / num;
      barH = percentSize * panelH;
      return true;
    }
    else {
      return false;


      //expand the body by the width of the bard
      //hide the bar

      //or

      //grey it out and make it non-interactable and only render the panel not the scrollbar
    }
  }

  void Hide_Scroll_Bar(Graphics::ScrollBar &scrollBar, const float &barWidth, float &bodyW, const bool &show) {
    if (show && scrollBar.show) {
      scrollBar.panel.w = barWidth;
      scrollBar.bar.w = barWidth;
      scrollBar.bar.x -= barWidth;
      scrollBar.panel.x -= barWidth;
      bodyW -= barWidth;
      scrollBar.show = true;
    }
    else if (!show && !scrollBar.show) {
      scrollBar.panel.x += barWidth;
      scrollBar.panel.w = 0.0f;
      scrollBar.bar.x += barWidth;
      scrollBar.bar.w = 0.0f;
      bodyW += barWidth;
      scrollBar.show = false;
    }
  }

  void Update(App::App &app, Graphics::ScrollBar &scrollBar, float &scrollBarHeight, const int &numElements,float &bodyW) {
    int maxElementsToDisplay = (int)(scrollBar.panel.h / (scrollBar.elementHeight + scrollBar.elementSpacing)) + 1;
    auto hideScrollBar = Set_Bar_Size(maxElementsToDisplay, numElements, scrollBar.panel.h, scrollBarHeight);
    app.panel = Graphics::Set_Panels(app.context.window, app.uiPanels);
    Hide_Scroll_Bar(scrollBar, app.uiPanels.scrollWidth, bodyW, hideScrollBar);
    App::Set_Textures(app);
  }

  bool Scroll(App::App &app, Graphics::ScrollBar &scrollBar, float &scrollBarPosition, float &scrollBarHeight, const int &numElements, const Sint32 &scroll, float &bodyW) {

    if (scroll < 0) {
      scrollBarPosition += 10.0f;
      if (scrollBarPosition > scrollBar.panel.h - scrollBar.bar.h)
        scrollBarPosition = scrollBar.panel.h - scrollBar.bar.h;
    }
    else {
      scrollBarPosition -= 10.0f;
      if (scrollBarPosition < 0.0f) scrollBarPosition = 0.0f;
    }

    Update(app, scrollBar, scrollBarHeight, numElements, bodyW);
    return true;
  }

  bool Update_ScrollBar(App::App &app) {
    //only need to update if the left mouse is held down

    if (app.selected == App::SCROLLBAR_LEFT) {
      app.uiPanels.scrollBarLeftY = (Mouse::Cursor_Point().y - app.panel.mainPanel.left.scroll.panel.y) - app.cachedScrollBarPosition;
      if (app.uiPanels.scrollBarLeftY < 0.0f) app.uiPanels.scrollBarLeftY = 0.0f;
      else if (app.uiPanels.scrollBarLeftY > app.panel.mainPanel.left.scroll.panel.h - app.panel.mainPanel.left.scroll.bar.h)
        app.uiPanels.scrollBarLeftY = app.panel.mainPanel.left.scroll.panel.h - app.panel.mainPanel.left.scroll.bar.h;
      return true;
    }
    else if (app.selected == App::SCROLLBAR_RIGHT) {
      app.uiPanels.scrollBarRightY = Mouse::Cursor_Point().y - app.panel.mainPanel.right.scroll.panel.y - app.cachedScrollBarPosition;
      if (app.uiPanels.scrollBarRightY < 0.0f) app.uiPanels.scrollBarRightY = 0.0f;
      else if (app.uiPanels.scrollBarRightY > app.panel.mainPanel.right.scroll.panel.h - app.panel.mainPanel.right.scroll.bar.h)
        app.uiPanels.scrollBarRightY = app.panel.mainPanel.right.scroll.panel.h - app.panel.mainPanel.right.scroll.bar.h;
      return true;
    }
    else if (app.selected == App::SCROLLBAR_FIXTURES) {
      app.uiPanels.scrollBarFixturesY = Mouse::Cursor_Point().y - app.panel.mainPanel.center.shapes.scroll.panel.y - app.cachedScrollBarPosition;
      if (app.uiPanels.scrollBarFixturesY < 0.0f) app.uiPanels.scrollBarFixturesY = 0.0f;
      else if (app.uiPanels.scrollBarFixturesY > app.panel.mainPanel.center.shapes.scroll.panel.h - app.panel.mainPanel.center.shapes.scroll.bar.h)
        app.uiPanels.scrollBarFixturesY = app.panel.mainPanel.center.shapes.scroll.panel.h - app.panel.mainPanel.center.shapes.scroll.bar.h;
      return true;
    }

    //place the list (for render and selection) at an offset proportional to the position of the scroll bar

    //set on button release
    return false;
  }

  bool Update_Expander(App::App &app) {
    if (app.selected == App::EXPANDER_LEFT) {
      app.uiPanels.leftPanelWidth = Mouse::Cursor_Point().x;
      if (app.uiPanels.leftPanelWidth < 100.0f) app.uiPanels.leftPanelWidth = 100.0f;
      else if (app.uiPanels.leftPanelWidth > app.uiPanels.window_w / 2.0f) app.uiPanels.leftPanelWidth = app.uiPanels.window_w / 2.0f;
      if (app.panel.mainPanel.center.panel.w < 450.0f) app.uiPanels.rightPanelWidth -= (450.0f - app.panel.mainPanel.center.panel.w);
      return true;
    }

    else if (app.selected == App::EXPANDER_RIGHT) {
      app.uiPanels.rightPanelWidth = app.uiPanels.window_w - Mouse::Cursor_Point().x;
      if (app.uiPanels.rightPanelWidth < 250.0f) app.uiPanels.rightPanelWidth = 250.0f;
      else if (app.uiPanels.rightPanelWidth > (app.uiPanels.window_w / 2.0f)) app.uiPanels.rightPanelWidth = app.uiPanels.window_w / 2.0f;
      if (app.panel.mainPanel.center.panel.w < 450.0f) app.uiPanels.leftPanelWidth -= (450.0f - app.panel.mainPanel.center.panel.w);
      return true;
    }

    else if (app.selected == App::EXPANDER_FIXTURES) {
      app.uiPanels.shapeListWidth = app.uiPanels.window_w - Mouse::Cursor_Point().x - app.uiPanels.rightPanelWidth;
      if (app.uiPanels.shapeListWidth < 125.0f) app.uiPanels.shapeListWidth = 125.0f;
      else if (app.uiPanels.shapeListWidth > 400.0f) app.uiPanels.shapeListWidth = 400.0f;
      if (app.panel.mainPanel.center.panel.w < 450.0f) app.uiPanels.leftPanelWidth -= (450.0f - app.panel.mainPanel.center.panel.w);
      return true;
    }
    return false;
  }

  void Update_UI_Position(App::App &app) {
    if (app.selected == App::NONE)
      return;

    if (!Update_Expander(app))
      if (!Update_ScrollBar(app))
        return;

    app.panel = Graphics::Set_Panels(app.context.window, app.uiPanels);
    App::Set_Textures(app);
  }

}