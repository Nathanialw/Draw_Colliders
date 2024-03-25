#pragma once

//
// Created by nathanial on 3/18/24.
//

#ifndef BOX2D_COLLIDERS_SHAPE_H
#define BOX2D_COLLIDERS_SHAPE_H

#endif //BOX2D_COLLIDERS_SHAPE_H
#include "../../Graphics/graphics.h"

namespace Shape {

  enum shape {
    POINT,
    LINE,
    CIRCLE,
    AABB,
    POLYGON,
    SIZE,
  };


  struct Shape {
    std::vector<SDL_FPoint> vertices{};
    std::vector<bool> moving{};

    bool isSensor = false;
    int bounce = 0;
    int density = 0;
    int friction = 0;
    int restitution = 0;

    int group = 0;
  };

  struct Vertex {
    shape shape = SIZE;
    int indexPolygon = 0;
    int indexVertex = 0;
  };

  struct Fixture {
    shape shape = SIZE;
    int indexPolygon = 0;
  };

  struct Offsets {
    float x = 0.0f;
    float y = 0.0f;
    float r = 0.0f;
  };

  struct Shape_List {
    std::array<std::vector<std::string>, SIZE> shapeList;

    Shape_List () {
      shapeList[POINT].emplace_back("Points");
      shapeList[CIRCLE].emplace_back("Circles");
      shapeList[LINE].emplace_back("Lines");
      shapeList[AABB].emplace_back("AABBs");
      shapeList[POLYGON].emplace_back("Polygons");
    }
  };

  bool Set_Vertex(std::array<std::vector<Shape>, SIZE> &shapes, const Fixture &selectedShape, Vertex &vertex, SDL_FPoint &offset, const float &scale, bool &moveVertex, int &max);

  void Render_Shapes(SDL_Renderer *renderer, SDL_Texture* vertexTexture, Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const Fixture &selectedShape, Vertex &selectedVertex, Vertex &vertex, bool &moveVertex, SDL_FPoint &offset, bool moveImage,  bool zoomToMouse, const float &vertexRadius, SDL_FRect &image);
  Vertex Get_Shape(Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const float &vertexRadius, SDL_FRect &image, SDL_FPoint &offset, bool &moveImage, bool &zoomToMouse, const SDL_FRect &cursor, const Fixture &selectedShape);
  Vertex Get_Vertex(Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const float &vertexRadius, SDL_FRect &image, const SDL_FRect &cursor, SDL_FPoint &offset, bool &moveImage, bool &zoomToMouse);
}