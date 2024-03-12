//
// Created by nathanial on 3/9/24.
//

#include "core.h"
#include "../Input/mouse.h"

namespace App {

  void Init (App &app) {
    app.context = Graphics::CreateWindowAndRenderer();
    app.panel = Graphics::Set_Panels(app.context.window);

    Mouse::Set_Cursor(app, SDL_SYSTEM_CURSOR_ARROW);
  }

  void Close(App &app) {
    SDL_DestroyTexture(app.interface.center.texture.texture);
    for (auto &texture: app.interface.left.images) {
      SDL_DestroyTexture(texture.texture);
    }
    SDL_FreeCursor(app.cursor);
    SDL_DestroyRenderer(app.context.renderer);
    SDL_DestroyWindow(app.context.window);
    SDL_Quit();
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
      for (int j = 0; j < shape.circles[i].vertexes.size(); ++j) {
        auto vRect = Vertex_To_Rect(app, shape.circles[i].vertexes[j], o, shape.circles[i].moving[j]);
        if (SDL_HasIntersectionF(&vRect, &cursor)){
          shape.circles[i].moving[j] = true;
          return {Graphics::CIRCLE, i, j};
        }
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

}