//
// Created by nathanial on 3/10/24.
//
#include "iostream"

#include "../../Utils/log.h"
#include "SDL2/SDL.h"
#include "left.h"
#include "center.h"
#include "../../Graphics/text.h"

namespace Center::Center {
  int appMax = 10;
  int imageMoveMax = 20;

  int Create_Circle_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.circles.emplace_back(Circle::Create(app.panel.mainPanel.center));
    return 0;
  }
  int Create_Point_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.points.emplace_back(Point::Create(app.panel.mainPanel.center));
    return 1;
  }
  int Create_Polygon_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.polygons.emplace_back(Polygon::Create(app.panel.mainPanel.center));
    return 2;
  }
  int Create_Rect_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.rects.emplace_back(AABB::Create(app.panel.mainPanel.center));
    return 3;
  }
  int Create_Line_Button(App::App &app) {
    if (app.interface.center.texture.texture)
      app.interface.center.lineSegments.emplace_back(Line_Segment::Create(app.panel.mainPanel.center));
    return 4;
  }
  int Unused(App::App &app) {
//    Log(5);
    return 5;
  }

  std::array<Edit_Button, 6> editButtons = {
      Create_Point_Button,
      Create_Circle_Button,
      Create_Line_Button,
      Create_Rect_Button,
      Create_Polygon_Button,
      Unused,
  };

  int Click_Button(App::App &app, const int &i) {
    editButtons[i](app);
    return 0;
  }

  bool Set_Scale(App::App &app, Sint32 direction) {
    if (direction < 0)
      app.interface.center.texture.scale *= 0.9;
    if (direction > 0)
      app.interface.center.texture.scale /= 0.9;
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
    point.x = point.x - (float)app.offset.x / app.interface.center.texture.scale;
    point.y = point.y - (float)app.offset.y / app.interface.center.texture.scale;
    app.offset = {0, 0};
    app.moveVertex = false;
    return true;
  }

  bool Update_Image(App::App &app) {
    if (app.moveImage || app.moveVertex) {
      int x;
      int y;
      SDL_GetMouseState(&x, &y);
      app.offset = {app.initialPosition.x - x, app.initialPosition.y - y};
      return true;
    }
    return false;
  }

  bool Set(App::App &app, SDL_FPoint &point) {
    app.max = appMax;
    point.x = point.x + (float)app.offset.x;
    point.y = point.y + (float)app.offset.y;
    app.offset = {0, 0};
    app.moveImage = false;
    return true;
  }

  bool Set_Vertex(App::App &app) {
    switch (app.vertex.shape) {
      case Graphics::AABB: {
        if (!app.interface.center.rects.empty())
          if (!app.interface.center.rects[app.vertex.indexPolygon].vertexes.empty()) {
            Set_vertex(app, app.interface.center.rects[app.vertex.indexPolygon].vertexes[app.vertex.indexVertex]);
            app.interface.center.rects[app.vertex.indexPolygon].moving[app.vertex.indexVertex] = false;
          }
        return true;
      }
      case Graphics::POINT: {
        if (!app.interface.center.points.empty()) {
          Set_vertex(app, app.interface.center.points[app.vertex.indexPolygon].vertex);
          app.interface.center.points[app.vertex.indexPolygon].moving = false;
        }
        return true;
      }
      case Graphics::POLYGON: {
        if (!app.interface.center.polygons.empty())
          if (!app.interface.center.polygons[app.vertex.indexPolygon].vertexes.empty()) {
            Set_vertex(app, app.interface.center.polygons[app.vertex.indexPolygon].vertexes[app.vertex.indexVertex]);
            app.interface.center.polygons[app.vertex.indexPolygon].moving[app.vertex.indexVertex] = false;
          }
        return true;
      }
      case Graphics::CIRCLE: {
        if (!app.interface.center.circles.empty())
          if (!app.interface.center.circles[app.vertex.indexPolygon].vertexes.empty()) {
            Set_vertex(app, app.interface.center.circles[app.vertex.indexPolygon].vertexes[app.vertex.indexVertex]);
            app.interface.center.circles[app.vertex.indexPolygon].moving[app.vertex.indexVertex] = false;
          }
        return true;
      }
      case Graphics::LINE: {
        if (!app.interface.center.lineSegments.empty())
          if (!app.interface.center.lineSegments[app.vertex.indexPolygon].vertexes.empty()) {
            Set_vertex(app, app.interface.center.lineSegments[app.vertex.indexPolygon].vertexes[app.vertex.indexVertex]);
            app.interface.center.lineSegments[app.vertex.indexPolygon].moving[app.vertex.indexVertex] = false;
          }
        return true;
      }
      case Graphics::SIZE: {
        return false;
      }
    }
  }

  void Render_Lines(App::App &app) {
    auto o = App::Calc_Offset(app);

    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);
    for (const auto &line:app.interface.center.lineSegments) {
      std::vector<SDL_FPoint> points;
      for (int i = 0; i < line.vertexes.size(); ++i) {
        SDL_FRect rect = Vertex_To_Rect(app, line.vertexes[i], o, line.moving[i]);
        points.push_back({rect.x + o.r, rect.y + o.r});
        /*          a test for displaying vertex coordinates          */
        //get the updated vertex value for this while its moving
        int x = (int)line.vertexes[i].x;
        int y = (int)line.vertexes[i].y;
        std::string coords = std::to_string(x) + ", " + std::to_string(y);
        Text::Render(app.context.renderer, app.context.font, coords.c_str(), rect.x + (o.r * 2), rect.y - o.r);
        /*                                                            */
        SDL_RenderFillRectF(app.context.renderer, &rect);
      }
      SDL_RenderDrawLinesF(app.context.renderer, points.data(), (int)points.size());
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Circles(App::App &app) {
    auto o = App::Calc_Offset(app);

    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);
    for (const auto &circle:app.interface.center.circles) {
      std::vector<SDL_FPoint> points;
      for (int i = 0; i < circle.vertexes.size(); ++i) {
        SDL_FRect rect = Vertex_To_Rect(app, circle.vertexes[i], o, circle.moving[i]);
        SDL_RenderFillRectF(app.context.renderer, &rect);
        points.push_back({rect.x + o.r, rect.y + o.r});
      }
      SDL_RenderDrawLineF(app.context.renderer, points[(int)points.size() - 1].x, points[(int)points.size() - 1].y, points[0].x, points[0].y);
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Polygons(App::App &app) {
    auto o = App::Calc_Offset(app);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);

    for (const auto &polygon : app.interface.center.polygons) {
      std::vector<SDL_FPoint> points;
      for (int i = 0; i < polygon.vertexes.size(); ++i) {
        SDL_FRect rect = Vertex_To_Rect(app, polygon.vertexes[i], o, polygon.moving[i]);
        points.push_back({rect.x + o.r, rect.y + o.r});
        SDL_RenderFillRectF(app.context.renderer, &rect);
      }
      SDL_RenderDrawLinesF(app.context.renderer, points.data(), (int)points.size());
      if (points.size() > 2)
        SDL_RenderDrawLineF(app.context.renderer, points[(int)points.size() - 1].x, points[(int)points.size() - 1].y, points[0].x, points[0].y);
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Rects(App::App &app) {
    auto o = App::Calc_Offset(app);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);

    for (const auto &aabb:app.interface.center.rects) {
      std::vector<SDL_FPoint> points;
      for (int i = 0; i < aabb.vertexes.size(); ++i) {
        SDL_FRect rect = Vertex_To_Rect(app, aabb.vertexes[i], o, aabb.moving[i]);
        points.push_back({rect.x + o.r, rect.y + o.r});
        SDL_RenderFillRectF(app.context.renderer, &rect);
      }

      SDL_RenderDrawLinesF(app.context.renderer, points.data(), (int)points.size());
      if (points.size() > 2)
        SDL_RenderDrawLineF(app.context.renderer, points[(int)points.size() - 1].x, points[(int)points.size() - 1].y, points[0].x, points[0].y);
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Points(App::App &app) {
    auto o = App::Calc_Offset(app);
    SDL_SetRenderDrawColor(app.context.renderer, 255, 0, 0, 255);
    for (int i = 0; i < app.interface.center.points.size(); ++i) {
      SDL_FRect rect = Vertex_To_Rect(app, app.interface.center.points[i].vertex, o, app.interface.center.points[i].moving);
      SDL_RenderFillRectF(app.context.renderer, &rect);
    }
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
  }

  void Render_Image(App::App &app) {
    if (app.interface.center.texture.texture) {
      SDL_Point size;
      SDL_QueryTexture(app.interface.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
      float w = (float) size.x * app.interface.center.texture.scale;
      float h = (float) size.y * app.interface.center.texture.scale;
//      auto o = App::Calc_Offset(app);
//      SDL_FRect rect = Vertex_To_Rect(app, vertex, o);

      SDL_FRect rect = {
          (app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w)) / 2.0f),
          (app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h)) / 2.0f),
          w,
          h,
      };
      if (app.moveImage) {
        rect.x -= (float) app.offset.x;
        rect.y -= (float) app.offset.y;
      }
       rect.x -= (float) app.interface.center.texture.offset.x;
       rect.y -= (float) app.interface.center.texture.offset.y;

      SDL_RenderCopyF(app.context.renderer, app.interface.center.texture.texture, nullptr, &rect);
    }

    Render_Lines(app);
    Render_Circles(app);
    Render_Points(app);
    Render_Rects(app);
    Render_Polygons(app);
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