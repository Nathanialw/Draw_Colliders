//
// Created by nathanial on 3/11/24.
//

#include "polygon.h"
#include "SDL2_gxf/SDL2_gfxPrimitives.h"

namespace Polygon {

  Shape::Shape Create() {
    Shape::Shape polygon{};
    SDL_FPoint vertex;
    float radius = 50.0f;

    //topLeft
    vertex.x = -radius;
    vertex.y = -radius;
    polygon.vertices.emplace_back(vertex);
    polygon.moving.emplace_back(false);
    //bottomRight
    vertex.x = +radius;
    vertex.y = +radius;
    polygon.vertices.emplace_back(vertex);
    polygon.moving.emplace_back(false);
    //bottomLeft
    vertex.x = -radius;
    vertex.y = +radius;
    polygon.vertices.emplace_back(vertex);
    polygon.moving.emplace_back(false);
    return polygon;
  }


  void Render_Polygon(SDL_Renderer *renderer, const std::vector<SDL_FPoint> &points, const SDL_Color &shapeFill) {
    std::vector<double> xPolygonPoints;
    std::vector<double> yPolygonPoints;

//          SDL_FRect rect = {MAXFLOAT, MAXFLOAT, -MAXFLOAT, -MAXFLOAT};
    for (const auto &vertex: points) {
      xPolygonPoints.push_back(vertex.x);
      yPolygonPoints.push_back(vertex.y);
//            if (vertex.x < rect.x)
//              rect.x = vertex.x;
//            if (vertex.y < rect.y)
//              rect.y = vertex.y;
    }
//          for (const auto &vertex: points) {
//            if (fabs(vertex.x - rect.x) > rect.w)
//              rect.w = fabs(vertex.x - rect.x);
//            if (fabs(vertex.y - rect.y) > rect.h)
//              rect.h = fabs(vertex.y - rect.y);
//          }
//          int g = 0;
//          for (const auto &vertex: app.interface.center.shapes[k][i].moving) {
//            if (vertex) {
//              g++;
//            }
//          }
//
//          if ((g !=0 && g != app.interface.center.shapes[k][i].moving.size()) || !app.texture.shapes[app.interface.center.index][k][i]) {
//            SDL_DestroyTexture(app.texture.shapes[app.interface.center.index][k][i]);
//            app.texture.shapes[app.interface.center.index][k][i] = nullptr;
//            app.texture.shapes[app.interface.center.index][k][i] = Graphics::Render_Polygon_AABB(app.context.renderer, xPolygonPoints, yPolygonPoints, rect.w, rect.h, shapeFill);
//          }
//
//          SDL_RenderCopyF(app.context.renderer, app.texture.shapes[app.interface.center.index][k][i], nullptr, &rect);
    aaFilledPolygonRGBA(renderer, xPolygonPoints.data(), yPolygonPoints.data(), (int) yPolygonPoints.size(), shapeFill.r, shapeFill.g, shapeFill.b, shapeFill.a);
  }

}