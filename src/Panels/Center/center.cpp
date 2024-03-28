//
// Created by nathanial on 3/10/24.
//
#include "iostream"

#include "../../Utils/utils.h"
#include "SDL2/SDL.h"
#include "left.h"
#include "center.h"
#include "../../Graphics/text.h"
#include "../../Input/actions.h"
#include "../../Input/mouse.h"
#include "../../UI/scroll_bar.h"

namespace Center::Center {
  const int imageMoveMax = 20;
  const int appMax = 10;

  std::array<Action::Button, Graphics::ButtonBarSize> editButtons = {
      Action::Create_Point,
      Action::Create_Circle,
      Action::Create_Line,
      Action::Create_Rect,
      Action::Create_Polygon,
      Action::UNIMPLEMENTED,
      Action::Delete_Selected_Shape,
//    //maybe grey them out if a polygon is not selected
      Action::Create_Vertex_If_Polygon_Selected,
      Action::Delete_Vertex_If_Polygon_Selected,
  };

  int Click_Button(App::App &app, const int &i) {
    editButtons[i](app);
    return 0;
  }

  bool Set_Scale(App::App &app, Sint32 direction) {
    if (direction < 0) {
      if (app.interface.center.texture.scale >= 0.1f) {
        app.interface.center.texture.scale *= 0.9f;
        // by the offset from the origin and current mouse
        app.offset.x *= 0.9;
        app.offset.y *= 0.9;
//        app.zoom = true;
        return true;
      }
    }
    else if (direction > 0) {
      if (app.interface.center.texture.scale <= 10.f) {
        app.interface.center.texture.
        scale /= 0.9f;
        app.offset.x /= 0.9;
        app.offset.y /= 0.9;
//        app.zoom = true;
        return true;
      }
    }
//    app.zoom = false;
    return false;
  }

  bool Move(App::App &app) {
    app.max = imageMoveMax;
    SDL_GetMouseState(&app.initialPosition.x, &app.initialPosition.y);
    app.moveImage = true;
    return true;
  }

  bool Set(int &max, SDL_FPoint &offset, SDL_FPoint &point, bool &moveImage) {
    max = appMax;
    point.x = point.x + (float)offset.x;
    point.y = point.y + (float)offset.y;
    offset = {0.0f, 0.0f};
    moveImage = false;
    return true;
  }

  bool Move_Vertex(App::App &app) {
    app.max = imageMoveMax;
    SDL_GetMouseState(&app.initialPosition.x, &app.initialPosition.y);
    app.moveVertex = true;
    return true;
  }

  bool Update_Image(App::App &app) {
    if (app.moveImage || app.moveVertex || app.zoomToMouse) {
      if (app.vertex.shape == Shape::CIRCLE && app.vertex.indexVertex == 1) {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        auto r = (app.interface.center.shapes[Shape::CIRCLE][app.vertex.indexPolygon].vertices[1].y - app.interface.center.shapes[Shape::CIRCLE][app.vertex.indexPolygon].vertices[0].y) * app.interface.center.texture.scale;
        int oY = app.initialPosition.y - y;
        if (oY > r) oY = r;
        app.offset = {0, (float)oY};
        return true;
      }
      else {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        app.offset = {(float)app.initialPosition.x - (float)x, (float)app.initialPosition.y - (float)y};
        return true;
      }
    }
    return false;
  }


  void Render_Image(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);

    if (app.interface.center.texture.texture) {
      SDL_Point size;
      SDL_QueryTexture(app.interface.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
      float w = (float) size.x * app.interface.center.texture.scale;
      float h = (float) size.y * app.interface.center.texture.scale;

      SDL_FRect rect = {
          (app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w)) / 2.0f),
          (app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h)) / 2.0f),
          w,
          h,
      };
      if (app.moveImage || app.zoomToMouse) {
        rect.x -= (float) app.offset.x;
        rect.y -= (float) app.offset.y;
      }
       rect.x -= (float) app.interface.center.texture.offset.x;
       rect.y -= (float) app.interface.center.texture.offset.y;

      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
      SDL_RenderFillRectF(app.context.renderer, &rect);
      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
      SDL_RenderDrawRectF(app.context.renderer, &rect);
      Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);

      SDL_RenderCopyF(app.context.renderer, app.interface.center.texture.texture, nullptr, &rect);
    }
    Render_Shapes(app.context.renderer, app.texture.vertex, app.interface.center.texture, app.interface.center.shapes, app.selectedShape, app.selectedVertex, app.vertex, app.moveVertex, app.offset, app.moveImage,  app.zoomToMouse, app.vertexRadius, app.panel.mainPanel.center.image);
  }

  void Render_Shape_List_Names(App::App &app) {
    float x = app.panel.mainPanel.center.shapes.body.x;
    float y = app.panel.mainPanel.center.shapes.body.y;
    float w = 40.0f;
    int numElements= 0;

    for (int j = 0; j < Shape::SIZE; ++j)
      numElements += app.interface.shapeList.shapeList[j].size();

    int maxElementsToDisplay = (int)(app.panel.mainPanel.center.shapes.body.h / (app.panel.mainPanel.center.shapes.scroll.elementHeight + app.panel.mainPanel.center.shapes.scroll.elementSpacing)) + 1;
    Scroll_Bar::Set_Bar_Size(maxElementsToDisplay, numElements, app.panel.mainPanel.center.shapes.scroll.panel.h, app.uiPanels.scrollBarFixturesHeight);

    auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.center.shapes.scroll.bar.x,
                                    app.panel.mainPanel.center.shapes.scroll.panel.h,
                                    app.uiPanels.scrollBarFixturesY,
                                    app.uiPanels.scrollBarFixturesHeight,
                                    numElements,
                                    maxElementsToDisplay);

    for (int j = 0; j < Shape::SIZE; ++j) {
      int min = 0;
      if (index.min > app.interface.shapeList.shapeList[j].size()) {
        index.min -= app.interface.shapeList.shapeList[j].size();
        continue;
      }
      else {
        min = index.min;
        index.min = 0;
      }
      for (int i = min; i < app.interface.shapeList.shapeList[j].size(); ++i) {
        SDL_FRect dRect = {x, y + app.panel.mainPanel.center.shapes.scroll.elementSpacing, w - (app.panel.mainPanel.center.shapes.scroll.elementSpacing * 2.0f), app.panel.mainPanel.center.shapes.scroll.elementHeight};
        if (j == app.selectedShape.shape && (i - 1) == app.selectedShape.indexPolygon) {
          SDL_FRect rect = {
              dRect.x + app.panel.mainPanel.center.shapes.scroll.elementSpacing,
              dRect.y,
              app.panel.mainPanel.center.shapes.body.w - (app.panel.mainPanel.center.shapes.scroll.elementSpacing * 2.0f),
              dRect.h,
          };
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::SELECTED]);
          SDL_RenderFillRectF(app.context.renderer, &rect);
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::DARK_BORDERS]);
          SDL_RenderDrawRectF(app.context.renderer, &rect);
          Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
        }

        SDL_Rect rect = {
            (int)dRect.x + (int)app.panel.mainPanel.center.shapes.scroll.elementSpacing,
            (int)dRect.y,
            (int)app.panel.mainPanel.center.shapes.body.w - (int)(app.panel.mainPanel.center.shapes.scroll.elementSpacing * 3.0f),
            (int)dRect.h,
        };
        Text::Render(app.context.renderer, app.context.font, app.interface.shapeList.shapeList[j][i].c_str(), rect);
        y += app.panel.mainPanel.center.shapes.scroll.elementHeight + app.panel.mainPanel.center.shapes.scroll.elementSpacing;
      }
    }
  }

  Shape::Fixture Select_From_Shape_List_Names(App::App &app) {
    float x = app.panel.mainPanel.center.shapes.body.x;
    float y = app.panel.mainPanel.center.shapes.body.y;
    float w = 40.0f;
    float h = 25.0f;
    float spacing = 2.0f;
    int numElements= 0;

    for (int j = 0; j < Shape::SIZE; ++j)
      numElements += app.interface.shapeList.shapeList[j].size();

    int maxElementsToDisplay = (int)(app.panel.mainPanel.center.shapes.body.h / (h + spacing)) + 1;

    auto index = Scroll_Bar::Get_Min_Index(app.panel.mainPanel.center.shapes.scroll.bar.x,
                                    app.panel.mainPanel.center.shapes.scroll.panel.h,
                                    app.uiPanels.scrollBarFixturesY,
                                    app.uiPanels.scrollBarFixturesHeight,
                                    numElements,
                                    maxElementsToDisplay);

    for (int j = 0; j < Shape::SIZE; ++j) {
      int min = 0;
      if (index.min > app.interface.shapeList.shapeList[j].size()) {
        index.min -= app.interface.shapeList.shapeList[j].size();
        continue;
      }
      else {
        min = index.min;
        index.min = 0;
      }
      for (int i = min; i < app.interface.shapeList.shapeList[j].size(); ++i) {
        SDL_FRect dRect = {x, y, w - (spacing * 2.0f), h + spacing};
        SDL_FRect rect = {
            dRect.x + spacing,
            dRect.y,
            app.panel.mainPanel.center.shapes.body.w - (spacing * 2.0f),
            dRect.h,
        };
        auto cursor = Mouse::Cursor_Point();
        if (Point_FRect_Intersect(cursor, rect) && i != 0)
          return {Shape::shape(j), i - 1};

        y += h + spacing;
      }
    }
    return {Shape::SIZE, 0};
  }


  void Set_Expander(App::App &app) {
    if (app.selected == Mouse::NONE)
      return;

    app.selected = Mouse::NONE;
  }

  bool Scroll(App::App &app, const Sint32 &scroll) {
    int numElements= 0;
    for (int j = 0; j < Shape::SIZE; ++j)
      numElements += app.interface.shapeList.shapeList[j].size();

    Scroll_Bar::Scroll(app,
                       app.panel.mainPanel.center.shapes.scroll,
                       app.uiPanels.scrollBarFixturesY,
                       app.uiPanels.scrollBarFixturesHeight,
                       numElements,
                       scroll,
                       app.panel.mainPanel.center.shapes.body.w
                       );

    return true;
  }

  void Render_Shape_List(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.panel);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderLeft);

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.body);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.body);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderLeft);

    Render_Shape_List_Names(app);

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.expanderLeft);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.expanderLeft);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderLeft);

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.panel);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderRight);

    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.bar);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.bar);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderRight);
  };

  void Render_Left_Scroll(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.expanderLeft);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.expanderLeft);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderLeft);
  };

  void Render_Right_Scroll(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.expanderRight);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.expanderRight);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderRight);
  };

  void Render_Button_Bar(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.buttonBar.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.buttonBar.panel);

    for (const auto &btn: app.panel.mainPanel.center.buttonBar.buttons) {
      if (btn.texture) {
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
        SDL_RenderFillRectF(app.context.renderer, &btn.button);
        SDL_RenderCopyF(app.context.renderer, btn.texture, nullptr, &btn.button);
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
        SDL_RenderDrawRectF(app.context.renderer, &btn.button);
      }
    }
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.buttonBar);
  };

  void Render(App::App &app) {
    Scroll_Bar::Update_UI_Position(app);
    Render_Image(app);
    Render_Shape_List(app);
    Render_Left_Scroll(app);
    Render_Right_Scroll(app);
    Render_Button_Bar(app);
  }
}