//
// Created by nathanial on 3/11/24.
//

#include "circle.h"
#include "shape.h"
#include "../../../lib/SDL2_gxf/SDL2_gfxPrimitives.h"

namespace Circle {
  Shape::Shape Create() {
    Shape::Shape circle{};
    circle.vertices.resize(2);
    circle.vertices[0] = {0.0f, 0.0f};
    circle.vertices[1] = {0.0f, 50.0f};
    circle.moving.resize(2);
    circle.moving[0] = false;
    circle.moving[1] = false;

    return circle;
  };

  Shape::Shape Create(const float &x, const float &y, const float &y2) {
    Shape::Shape circle{};
    circle.vertices.resize(2);
    circle.vertices[0] = {x, y};
    circle.vertices[1] = {x, y2};
    circle.moving.resize(2);
    circle.moving[0] = false;
    circle.moving[1] = false;

    return circle;
  };


  void Render_Circle(SDL_Renderer *renderer, const std::vector<SDL_FPoint> &points, const SDL_Color &shapeFill,  const int &selectedShape, const int &selectedIndex, const int &i) {
    SDL_Color color;
    (selectedShape == Shape::CIRCLE && selectedIndex == i) ? color = {0,255,255,255} : color = {255,0,0,255};
    float r = points[1].y - points[0].y;

//          int g = 0;
//          for (const auto &vertex: app.interface.center.shapes[k][i].moving) {
//            if (vertex) {
//              g++;
//            }
//          }
//          if ((g !=0 && g != app.interface.center.shapes[k][i].moving.size() || !app.texture.shapes[app.interface.center.index][k][i])) {
//            SDL_DestroyTexture(app.texture.shapes[app.interface.center.index][k][i]);
//            app.texture.shapes[app.interface.center.index][k][i] = nullptr;
//            app.texture.shapes[app.interface.center.index][k][i] = Graphics::Render_Circle(app.context.renderer, points[0].x, points[0].y, r, shapeFill, color);
//          }
//
//          SDL_FRect rect = {
//              points[0].x - r,
//              points[0].y - r,
//              r * 2.0f,
//              r * 2.0f
//          };
//          SDL_RenderCopyF(app.context.renderer, app.texture.shapes[app.interface.center.index][k][i], nullptr, &rect);
    aaFilledPieRGBA(renderer, points[0].x, points[0].y, r, r, 0.0f, 360.0f, 0, shapeFill.r, shapeFill.g, shapeFill.b, shapeFill.a);
    thickCircleRGBA(renderer, points[0].x, points[0].y, r, color.r, color.g, color.b, color.a, 2);
  }


}