//
// Created by nathanial on 3/9/24.
//

#include "core.h"
#include "../Input/mouse.h"
#include "../Graphics/text.h"


namespace App {

  void Init (App &app) {
    app.context = Graphics::CreateWindowAndRenderer();
    app.panel = Graphics::Set_Panels(app.context.window);

    app.context.font = Text::Load_Font(app.context.renderer);
    Mouse::Set_Cursor(app, SDL_SYSTEM_CURSOR_ARROW);
  }

  void Close(App &app) {
    SDL_DestroyTexture(app.interface.center.texture.texture);
    for (auto &texture: app.interface.left.images) {
      SDL_DestroyTexture(texture.texture.texture);
    }
    SDL_FreeCursor(app.cursor);
    SDL_DestroyRenderer(app.context.renderer);
    SDL_DestroyWindow(app.context.window);
    SDL_Quit();
  }

  void New(App &app) {
    Data::Left newLeft;
    Data::Center newCenter;
    app.interface.left = newLeft;
    app.interface.center = newCenter;
  }

  Offsets Calc_Offset(const App &app) {
    SDL_Point size;
    SDL_QueryTexture(app.interface.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
    float w = (float) size.x * app.interface.center.texture.scale;
    float h = (float) size.y * app.interface.center.texture.scale;

    return {
        (app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w)) / 2.0f) + (w / 2.0f),
        (app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h)) / 2.0f) + (h / 2.0f),
        (float) app.vertexRadius * app.interface.center.texture.scale
    };
  }

  SDL_FPoint Offset_From_Image_Center(const App &app, const SDL_FPoint &point) {
    SDL_Point size;
    SDL_QueryTexture(app.interface.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
    float w = (float) size.x * app.interface.center.texture.scale;
    float h = (float) size.y * app.interface.center.texture.scale;

    auto x = (app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w)) / 2.0f) + (w / 2.0f);
    auto y = (app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h)) / 2.0f) + (h / 2.0f);
    auto gg = -(x - point.x) / app.interface.center.texture.scale;
    auto hh = -(y - point.y) / app.interface.center.texture.scale;
    gg += (float) app.interface.center.texture.offset.x / app.interface.center.texture.scale;
    hh += (float) app.interface.center.texture.offset.y / app.interface.center.texture.scale;
    return {gg, hh};
  }

  SDL_FRect Vertex_To_Rect(const App &app, const SDL_FPoint &vertex, const Offsets &o, bool moveVertex) {
    SDL_FRect rect;
    rect.x = (vertex.x * app.interface.center.texture.scale) + o.x - o.r;
    rect.y = (vertex.y * app.interface.center.texture.scale) + o.y - o.r;
    rect.w = o.r * 2.0f;
    rect.h = o.r * 2.0f;
    if (app.moveImage || moveVertex) {
      rect.x -= (float) app.offset.x;
      rect.y -= (float) app.offset.y;
    }
    rect.x -= (float) app.interface.center.texture.offset.x;
    rect.y -= (float) app.interface.center.texture.offset.y;
    return rect;
  }

  SDL_FPoint Vertex_To_Screen(const App &app, const SDL_FPoint &vertex, const Offsets &o, bool moveVertex) {
    SDL_FRect rect;
    rect.x = (vertex.x * app.interface.center.texture.scale) + o.x;
    rect.y = (vertex.y * app.interface.center.texture.scale) + o.y;
    if (app.moveImage || moveVertex) {
      rect.x -= (float) app.offset.x;
      rect.y -= (float) app.offset.y;
    }
    rect.x -= (float) app.interface.center.texture.offset.x;
    rect.y -= (float) app.interface.center.texture.offset.y;
    return {rect.x, rect.y};
  }

  Vertex Get_Vertex(App &app, const SDL_FRect &cursor) {
    auto o = Calc_Offset(app);
    auto &shape = app.interface.center;

    for (int i = 0; i < shape.points.size(); ++i) {
      auto vRect = Vertex_To_Rect(app, shape.points[i].vertex, o, shape.points[i].moving);
      if (SDL_HasIntersectionF(&vRect, &cursor)){
        shape.points[i].moving = true;
        return {Graphics::POINT, i, 0};
      }
    }
    for (int i = 0; i < shape.polygons.size(); ++i) {
      for (int j = 0; j < shape.polygons[i].vertexes.size(); ++j) {
        auto vRect = Vertex_To_Rect(app, shape.polygons[i].vertexes[j], o, shape.polygons[i].moving[i]);
        if (SDL_HasIntersectionF(&vRect, &cursor)) {
          shape.polygons[i].moving[j] = true;
          return {Graphics::POLYGON, i, j};
        }
      }
    }
    for (int i = 0; i < shape.rects.size(); ++i) {
      for (int j = 0; j < shape.rects[i].vertexes.size(); ++j) {
        auto vRect = Vertex_To_Rect(app, shape.rects[i].vertexes[j], o, shape.rects[i].moving[j]);
        if (SDL_HasIntersectionF(&vRect, &cursor)) {
          shape.rects[i].moving[j] = true;
          return {Graphics::AABB, i, j};
        }
      }
    }
    for (int i = 0; i < shape.circles.size(); ++i) {
      auto vRect = Vertex_To_Rect(app, shape.circles[i].vertexes[1], o, shape.circles[i].moving[1]);
      if (SDL_HasIntersectionF(&vRect, &cursor)){
        shape.circles[i].moving[1] = true;
        return {Graphics::CIRCLE, i, 1};
      }
    }
    for (int i = 0; i < shape.lineSegments.size(); ++i) {
      for (int j = 0; j < shape.lineSegments[i].vertexes.size(); ++j) {
        auto vRect = Vertex_To_Rect(app, shape.lineSegments[i].vertexes[j], o, shape.lineSegments[i].moving[j]);
        if (SDL_HasIntersectionF(&vRect, &cursor)) {
          shape.lineSegments[i].moving[j] = true;
          return {Graphics::LINE, i, j};
        }
      }
    }
    return {Graphics::SIZE, 0};
  }

  Vertex Get_Shape(App &app, const SDL_FRect &cursor) {
    auto o = Calc_Offset(app);
    auto &shape = app.interface.center;
    std::vector<SDL_FPoint> mouseVertexes;
    mouseVertexes.push_back({cursor.x, cursor.y});
    mouseVertexes.push_back({cursor.x + cursor.w, cursor.y});
    mouseVertexes.push_back({cursor.x + cursor.w, cursor.y + cursor.h});
    mouseVertexes.push_back({cursor.x , cursor.y + cursor.h});
    std::vector<SDL_FPoint> shapeVertexes;

    for (int i = 0; i < shape.polygons.size(); ++i) {
      for (int j = 0; j < shape.polygons[i].vertexes.size(); ++j) {
        SDL_FPoint v = Vertex_To_Screen(app, shape.polygons[i].vertexes[j], o, shape.polygons[i].moving[j]);
        shapeVertexes.emplace_back(v);
      }
      /// this one only works for convex polygons, need one for concave polygons
//      if (PolygonOverlap_SAT(mouseVertexes, shapeVertexes)) {
      if (Point_In_Polygon(mouseVertexes, shapeVertexes)) {
        for (auto && move : shape.polygons[i].moving)
          move = true;
        return {Graphics::POLYGON, i, -1};
      }
      shapeVertexes.clear();
    }

    for (int i = 0; i < shape.rects.size(); ++i) {
      for (int j = 0; j < shape.rects[i].vertexes.size(); ++j) {
        SDL_FPoint v = Vertex_To_Screen(app, shape.rects[i].vertexes[j], o, shape.rects[i].moving[j]);
        shapeVertexes.emplace_back(v);
      }
      if (PolygonOverlap_SAT(mouseVertexes, shapeVertexes)) {
        for (auto && move : shape.rects[i].moving)
          move = true;
        return {Graphics::AABB, i, -1};
      }
      shapeVertexes.clear();
    }

    for (int i = 0; i < shape.circles.size(); ++i) {
      for (int j = 0; j < shape.circles[i].vertexes.size(); ++j) {
        SDL_FPoint v = Vertex_To_Screen(app, shape.circles[i].vertexes[j], o, shape.circles[i].moving[j]);
        shapeVertexes.emplace_back(v);
      }
      if (Circle_Intersect(shapeVertexes[0].x, shapeVertexes[0].y, shapeVertexes[1].y - shapeVertexes[0].y, cursor)) {
        for (auto && move : shape.circles[i].moving)
          move = true;
        return {Graphics::CIRCLE, i, -1};
      }
      shapeVertexes.clear();
    }

    for (int i = 0; i < shape.lineSegments.size(); ++i) {
      for (int j = 0; j < shape.lineSegments[i].vertexes.size(); ++j) {
          SDL_FPoint v = Vertex_To_Screen(app, shape.lineSegments[i].vertexes[j], o, shape.lineSegments[i].moving[j]);
          shapeVertexes.emplace_back(v);
        }
        if (PolygonOverlap_SAT(mouseVertexes, shapeVertexes)) {
          for (auto && move : shape.lineSegments[i].moving)
            move = true;
          return {Graphics::LINE, i, -1};
        }
        shapeVertexes.clear();
    }
    return {Graphics::SIZE, 0};
  }

}