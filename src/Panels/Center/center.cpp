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

namespace Center::Center {
  int appMax = 10;
  int imageMoveMax = 20;

  int Create_Circle_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.shapes[Graphics::CIRCLE].emplace_back(Circle::Create(app.panel.mainPanel.center));
    return 0;
  }
  int Create_Point_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.shapes[Graphics::POINT].emplace_back(Point::Create(app.panel.mainPanel.center));
    return 1;
  }
  int Create_Polygon_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.shapes[Graphics::POLYGON].emplace_back(Polygon::Create(app.panel.mainPanel.center));
    return 2;
  }
  int Create_Rect_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.shapes[Graphics::AABB].emplace_back(AABB::Create());
    return 3;
  }
  int Create_Line_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.shapes[Graphics::LINE].emplace_back(Line_Segment::Create(app.panel.mainPanel.center));
    return 4;
  }
  int Delete_Selected_Shape(App::App &app) {
    Action::Delete_Shape(app);
    return 5;
  }
  int Create_Vertex_If_Polygon_Selected(App::App &app) {
    Action::Add_Vertex_Center(app);
    return 5;
  }
  int Delete_Vertex_If_Polygon_Selected(App::App &app) {
    Action::Delete_Vertex(app);
    return 5;
  }
  int Unused(App::App &app) {
    Action::Delete_Vertex(app);
    return 5;
  }

  std::array<Edit_Button, Graphics::ButtonBarSize> editButtons = {
      Create_Point_Button,
      Create_Circle_Button,
      Create_Line_Button,
      Create_Rect_Button,
      Create_Polygon_Button,
      Unused,
      Delete_Selected_Shape,
      //maybe grey them out if a polygon is not selected
      Create_Vertex_If_Polygon_Selected,
      Delete_Vertex_If_Polygon_Selected,
  };

  int Click_Button(App::App &app, const int &i) {
    editButtons[i](app);
    return 0;
  }

  bool Set_Scale(App::App &app, Sint32 direction) {
    if (direction < 0) {
      app.interface.center.texture.scale *= 0.9;
      // by the offset from the origin and current mouse
      app.offset.x *= 0.9f;
      app.offset.y *= 0.9f;
//      app.zoomToMouse = true;
    }
    if (direction > 0) {
      app.interface.center.texture.scale /= 0.9;
      app.offset.x /= 0.9f;
      app.offset.y /= 0.9f;
//      app.zoomToMouse = true;
    }

    return true;
  }

  bool Move(App::App &app) {
    app.max = imageMoveMax;
    SDL_GetMouseState(&app.initialPosition.x, &app.initialPosition.y);
    app.moveImage = true;
    return true;
  }

  bool Move_Vertex(App::App &app) {
    app.max = imageMoveMax;
    SDL_GetMouseState(&app.initialPosition.x, &app.initialPosition.y);
    app.moveVertex = true;
    return true;
  }

  bool Set_vertex(App::App &app, SDL_FPoint &point) {
    app.max = appMax;
    point.x = point.x - ((float)app.offset.x / app.interface.center.texture.scale);
    point.y = point.y - ((float)app.offset.y / app.interface.center.texture.scale);
    app.moveVertex = false;
    return true;
  }

  bool Update_Image(App::App &app) {
    if (app.moveImage || app.moveVertex || app.zoomToMouse) {
      if (app.vertex.shape == Graphics::CIRCLE && app.vertex.indexVertex == 1) {
        int x;
        int y;
        SDL_GetMouseState(&x, &y);
        auto r = (app.interface.center.shapes[Graphics::CIRCLE][app.vertex.indexPolygon].vertices[1].y - app.interface.center.shapes[Graphics::CIRCLE][app.vertex.indexPolygon].vertices[0].y) * app.interface.center.texture.scale;
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

  bool Set(App::App &app, SDL_FPoint &point) {
    app.max = appMax;
    point.x = point.x + (float)app.offset.x;
    point.y = point.y + (float)app.offset.y;
    app.offset = {0.0f, 0.0f};
    app.moveImage = false;
    return true;
  }

  bool Set_Rect_Vertexes(App::App &app) {
    for (int i = 0; i < app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices.size() ; ++i) {
      if (app.vertex.indexVertex == 0) { //top left
        if (i == 1)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[1].y -= app.offset.y / app.interface.center.texture.scale;
        else if (i == 3)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[3].x -= app.offset.x / app.interface.center.texture.scale;
      }
      else if (app.vertex.indexVertex == 1) { //top right
        if (i == 0)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[0].y -= app.offset.y / app.interface.center.texture.scale;
        else if (i == 2)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[2].x -= app.offset.x / app.interface.center.texture.scale;
      }
      else if (app.vertex.indexVertex == 2) { //bottom right
        if (i == 1)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[1].x -= app.offset.x / app.interface.center.texture.scale;
        else if (i == 3)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[3].y -= app.offset.y / app.interface.center.texture.scale;
      }
      else if (app.vertex.indexVertex == 3) { // bottom left
        if (i == 2)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[2].y -= app.offset.y / app.interface.center.texture.scale;
        else if (i == 0)
          app.interface.center.shapes[Graphics::AABB][app.vertex.indexPolygon].vertices[0].x -= app.offset.x / app.interface.center.texture.scale;
      }
    }
    return true;
  }

  bool Set_Vertex(App::App &app) {
    if (app.vertex.shape == Graphics::SIZE) {
      return false;
    }

    if (!app.interface.center.shapes[app.vertex.shape].empty()) {
      if (!app.interface.center.shapes[app.vertex.shape][app.vertex.indexPolygon].vertices.empty()) {
        if (app.vertex.indexVertex == -1) {
          for (int i = 0; i < app.interface.center.shapes[app.selectedShape.shape][app.vertex.indexPolygon].vertices.size(); ++i) {
            Set_vertex(app, app.interface.center.shapes[app.selectedShape.shape][app.vertex.indexPolygon].vertices[i]);
            app.interface.center.shapes[app.selectedShape.shape][app.vertex.indexPolygon].moving[i] = false;
          }
        } else {
          if (app.selectedVertex.shape == Graphics::AABB)
            Set_Rect_Vertexes(app);
          Set_vertex(app, app.interface.center.shapes[app.vertex.shape][app.vertex.indexPolygon].vertices[app.vertex.indexVertex]);
          app.interface.center.shapes[app.vertex.shape][app.vertex.indexPolygon].moving[app.vertex.indexVertex] = false;
        }
      }
      app.offset = {0, 0};
      app.vertex = {Graphics::SIZE, 0, 0};
      return true;
    }
    return false;
  }

  void Set_Color(App::App &app, const int &i, const Graphics::Shape &shape) {
    if (app.selectedShape.shape == shape && app.selectedShape.indexPolygon == i)
      SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
  }

  void Reset_Color(App::App &app, const int &i, const Graphics::Shape &shape) {
    if (app.selectedShape.shape == shape && app.selectedShape.indexPolygon == i)
      SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);
  }

  void Render_Lines(App::App &app) {
    auto o = App::Calc_Offset(app);

    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);
    for (int i = 0; i < app.interface.center.shapes[Graphics::LINE].size(); ++i) {
      Set_Color(app, i, Graphics::LINE);
      auto &line = app.interface.center.shapes[Graphics::LINE][i];
      std::vector<SDL_FPoint> points;
      for (int j = 0; j < line.vertices.size(); ++j) {
        SDL_FRect rect = Vertex_To_Rect(app, line.vertices[j], o, line.moving[j]);
        points.push_back({rect.x + o.r, rect.y + o.r});
        /*          a test for displaying vertex coordinates          */
        //get the updated vertex value for this while its moving
        int x = (int)line.vertices[j].x;
        int y = (int)line.vertices[j].y;

        /*                                                            */
        SDL_RenderFillRectF(app.context.renderer, &rect);
      }
      SDL_RenderDrawLinesF(app.context.renderer, points.data(), (int)points.size());
      Reset_Color(app, i, Graphics::LINE);
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Update_Vertex_Render(App::App &app, SDL_FRect &rect, const int &i, const int &j) {
    if (app.moveVertex && app.vertex.shape == Graphics::AABB) {
      if (app.vertex.indexPolygon == i) {
        if (app.vertex.indexVertex == 0) { //top left
          if (j == 1)
            rect.y -= app.offset.y;
          else if (j == 3)
            rect.x -= app.offset.x;
        }
        if (app.vertex.indexVertex == 1) { //top right
          if (j == 0)
            rect.y -= app.offset.y;
          else if (j == 2)
            rect.x -= app.offset.x;
        }
        if (app.vertex.indexVertex == 2) { //bottom right
          if (j == 1)
            rect.x -= app.offset.x;
          else if (j == 3)
            rect.y -= app.offset.y;
        }
        if (app.vertex.indexVertex == 3) { // bottom left
          if (j == 2)
            rect.y -= app.offset.y;
          else if (j == 0)
            rect.x -= app.offset.x;
        }
      }
    }
  }

  void Render_Shapes(App::App &app) {
    auto o = App::Calc_Offset(app);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);

    for (int k = 0; k < Graphics::Shape::SIZE; ++k) {
      for (int i = 0; i < app.interface.center.shapes[k].size(); ++i) {
        std::vector<SDL_FPoint> points;
        Set_Color(app, i, (Graphics::Shape)k);
        for (int j = 0; j < app.interface.center.shapes[k][i].vertices.size(); ++j) {
          SDL_FRect rect = Vertex_To_Rect(app, app.interface.center.shapes[k][i].vertices[j], o, app.interface.center.shapes[k][i].moving[j]);
          //updates rect vertices position, needs to be moved
          if (k == Graphics::Shape::AABB)
            Update_Vertex_Render(app, rect, i, j);

          points.push_back({rect.x + o.r, rect.y + o.r});
          SDL_RenderFillRectF(app.context.renderer, &rect);

          //renders coordinates text, maybe needs to be moved
          int x = (int)app.interface.center.shapes[k][i].vertices[j].x;
          int y = (int)app.interface.center.shapes[k][i].vertices[j].y;
          std::string coords = std::to_string(x) + ", " + std::to_string(y);
          Text::Render(app.context.renderer, app.context.font, coords.c_str(), rect.x + (o.r * 2), rect.y - o.r);
        }

        SDL_RenderDrawLinesF(app.context.renderer, points.data(), (int)points.size());
        if (points.size() > 2)
          SDL_RenderDrawLineF(app.context.renderer, points[(int)points.size() - 1].x, points[(int)points.size() - 1].y, points[0].x, points[0].y);
        Reset_Color(app, i, (Graphics::Shape)k);
      }
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Image(App::App &app) {
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

      SDL_RenderCopyF(app.context.renderer, app.interface.center.texture.texture, nullptr, &rect);
    }
    Render_Shapes(app);
  }

  void Render_Shape_List(App::App &app) {
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 155, 155, 155, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderLeft);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 55, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderRight);

    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.bar);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 55, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.mainPanel.center.shapes.scroll.bar);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.expanderRight);
  };

  void Render_Left_Scroll(App::App &app) {
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
    SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 0, 255);
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
    Render_Shape_List(app);
    Render_Left_Scroll(app);
    Render_Right_Scroll(app);
    Render_Button_Bar(app);
  }
}