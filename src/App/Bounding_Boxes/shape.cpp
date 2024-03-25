
//
// Created by nathanial on 3/18/24.
//
#include "../../Graphics/graphics.h"
#include "shape.h"
#include "circle.h"
#include "polygon.h"
#include "../../Utils/utils.h"

namespace Shape {
  const int appMax = 10;

  bool Set_vertex(int &max, SDL_FPoint &offset, SDL_FPoint &point, bool &moveVertex, const float &scale) {
    max = appMax;
    point.x = point.x - ((float)offset.x / scale);
    point.y = point.y - ((float)offset.y / scale);
    moveVertex = false;
    return true;
  }
  bool Set_Rect_Vertexes(std::array<std::vector<Shape>, SIZE> &shapes, const Vertex &vertex, const SDL_FPoint &offset, const float &scale) {
    for (int i = 0; i < shapes[AABB][vertex.indexPolygon].vertices.size() ; ++i) {
      switch (vertex.indexVertex) {

        case 0: { //top left
          if (i == 1)
            shapes[AABB][vertex.indexPolygon].vertices[1].y -= offset.y / scale;
          else if (i == 3)
            shapes[AABB][vertex.indexPolygon].vertices[3].x -= offset.x / scale;
          break;
        }
        case 1: { //top right
          if (i == 0)
            shapes[AABB][vertex.indexPolygon].vertices[0].y -= offset.y / scale;
          else if (i == 2)
            shapes[AABB][vertex.indexPolygon].vertices[2].x -= offset.x / scale;
          break;
        }
        case 2: { //bottom right
          if (i == 1)
            shapes[AABB][vertex.indexPolygon].vertices[1].x -= offset.x / scale;
          else if (i == 3)
            shapes[AABB][vertex.indexPolygon].vertices[3].y -= offset.y / scale;
          break;
        }
        case 3: { // bottom left
          if (i == 2)
            shapes[AABB][vertex.indexPolygon].vertices[2].y -= offset.y / scale;
          else if (i == 0)
            shapes[AABB][vertex.indexPolygon].vertices[0].x -= offset.x / scale;
          break;
        }
      }
    }
    return true;
  }
  bool Set_Vertex(std::array<std::vector<Shape>, SIZE> &shapes, const Fixture &selectedShape, Vertex &vertex, SDL_FPoint &offset, const float &scale, bool &moveVertex, int &max) {
    if (vertex.shape == SIZE) {
      return false;
    }

    if (!shapes[vertex.shape].empty()) {
      if (!shapes[vertex.shape][vertex.indexPolygon].vertices.empty()) {
        if (vertex.indexVertex == -1 && selectedShape.indexPolygon < shapes[selectedShape.shape].size()) {
          if (selectedShape.shape == SIZE)
            return false;
          for (int i = 0; i < shapes[selectedShape.shape][selectedShape.indexPolygon].vertices.size(); ++i) {
            Set_vertex(max, offset, shapes[selectedShape.shape][selectedShape.indexPolygon].vertices[i], moveVertex, scale);
            shapes[selectedShape.shape][vertex.indexPolygon].moving[i] = false;
          }
        } else {
          if (selectedShape.shape == AABB)
            Set_Rect_Vertexes(shapes, vertex, offset, scale);
          Set_vertex(max, offset, shapes[vertex.shape][vertex.indexPolygon].vertices[vertex.indexVertex], moveVertex, scale);
          shapes[vertex.shape][vertex.indexPolygon].moving[vertex.indexVertex] = false;
        }
      }
      offset = {0, 0};
      vertex = {SIZE, 0, 0};
      return true;
    }
    return false;
  }
  void Set_Color(SDL_Renderer *renderer, const Fixture &selectedShape, const int &i, const shape &shape) {
    if (selectedShape.shape == shape && selectedShape.indexPolygon == i)
      Graphics::Set_Render_Draw_Color(renderer, Graphics::color[Graphics::SHAPESELECTED]);
    else
      Graphics::Set_Render_Draw_Color(renderer, Graphics::color[Graphics::SHAPE]);
  }
  void Reset_Color(SDL_Renderer *renderer, const Fixture &selectedShape, const int &i, const shape &shape) {
    if (selectedShape.shape == shape && selectedShape.indexPolygon == i)
      Graphics::Set_Render_Draw_Color(renderer, Graphics::color[Graphics::SHAPESELECTED]);
  }
  void Update_Vertex_Render(SDL_FRect &rect, Vertex &vertex, bool &moveVertex, SDL_FPoint &offset, const int &i, const int &j) {
    if (moveVertex && vertex.shape == AABB) {
      if (vertex.indexPolygon == i) {
        if (vertex.indexVertex == 0) { //top left
          if (j == 1)
            rect.y -= offset.y;
          else if (j == 3)
            rect.x -= offset.x;
        }
        if (vertex.indexVertex == 1) { //top right
          if (j == 0)
            rect.y -= offset.y;
          else if (j == 2)
            rect.x -= offset.x;
        }
        if (vertex.indexVertex == 2) { //bottom right
          if (j == 1)
            rect.x -= offset.x;
          else if (j == 3)
            rect.y -= offset.y;
        }
        if (vertex.indexVertex == 3) { // bottom left
          if (j == 2)
            rect.y -= offset.y;
          else if (j == 0)
            rect.x -= offset.x;
        }
      }
    }
  }
  SDL_FRect Vertex_To_Rect(Graphics::Image &texture, const SDL_FPoint &vertex, const Offsets &o, SDL_FPoint &offset, bool moveVertex,  bool moveImage,  bool zoomToMouse) {
    SDL_FRect rect;
    rect.x = (vertex.x * texture.scale) + o.x - o.r;
    rect.y = (vertex.y * texture.scale) + o.y - o.r;
    rect.w = o.r * 2.0f;
    rect.h = o.r * 2.0f;
    if (moveImage || moveVertex || zoomToMouse) {
      rect.x -= (float) offset.x;
      rect.y -= (float) offset.y;
    }
    rect.x -= (float) texture.offset.x;
    rect.y -= (float) texture.offset.y;
    return rect;
  }
  void Render_Shape(SDL_Renderer *renderer, SDL_Texture* vertexTexture, Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const Fixture &selectedShape, Vertex &selectedVertex, Vertex &vertex, bool &moveVertex, SDL_FPoint &offset, Offsets &o, const int &k, const int &i, const SDL_Color &shapeFill,  bool moveImage,  bool zoomToMouse) {
    std::vector<SDL_FPoint> points;

    Set_Color(renderer, selectedShape, i, (shape)k);
    for (int j = 0; j < shapes[k][i].vertices.size(); ++j) {
      SDL_FRect rect = Vertex_To_Rect(texture, shapes[k][i].vertices[j], o, offset, shapes[k][i].moving[j], moveImage, zoomToMouse);
      //updates rect vertices position, needs to be moved
      if (k == AABB)
        Update_Vertex_Render(rect, vertex, moveVertex, offset, i, j);
      points.push_back({rect.x + o.r, rect.y + o.r});

      //need to not scale the size of the vertices on zoom so they stay the same size always
      if (selectedVertex.shape == k && selectedVertex.indexPolygon == i && selectedVertex.indexVertex == j)
        SDL_SetTextureColorMod(vertexTexture, 0, 255, 0);
      else if (selectedShape.shape == k && selectedShape.indexPolygon == i)
        SDL_SetTextureColorMod(vertexTexture, 0, 255, 255);
      else
        SDL_SetTextureColorMod(vertexTexture, 255, 0, 0);

      SDL_RenderCopyF(renderer, vertexTexture, nullptr, &rect);
      Set_Color(renderer, selectedShape, i, (shape)k);

      //renders coordinates text, maybe needs to be moved
//      int x = (int)app.interface.center.shapes[k][i].vertices[j].x;
//      int y = (int)app.interface.center.shapes[k][i].vertices[j].y;
//      std::string coords = std::to_string(x) + ", " + std::to_string(y);
//      Text::Render(renderer, app.context.font, coords.c_str(), rect.x + (o.r * 2), rect.y - o.r);
    }

    if (k == CIRCLE) {
      Circle::Render_Circle(renderer, points, shapeFill, selectedShape.shape, selectedShape.indexPolygon, i);
    }

    else if (k == POLYGON || k == AABB) {
      Polygon::Render_Polygon(renderer, points, shapeFill);
    }
    Set_Color(renderer, selectedShape, i, (shape)k);
    SDL_RenderDrawLinesF(renderer, points.data(), (int)points.size());
    if (points.size() > 2)
      SDL_RenderDrawLineF(renderer, points[(int)points.size() - 1].x, points[(int)points.size() - 1].y, points[0].x, points[0].y);
    Reset_Color(renderer, selectedShape, i, (shape)k);
  }
  Offsets Calc_Offset(Graphics::Image &texture, const float &vertexRadius, SDL_FRect &image) {
    SDL_Point size;
    SDL_QueryTexture(texture.texture, nullptr, nullptr, &size.x, &size.y);
    float w = (float) size.x * texture.scale;
    float h = (float) size.y * texture.scale;

    return {
        (image.x + ((image.w - w)) / 2.0f) + (w / 2.0f),
        (image.y + ((image.h - h)) / 2.0f) + (h / 2.0f),
        (float) vertexRadius * texture.scale
    };
  }
  void Render_Shapes(SDL_Renderer *renderer, SDL_Texture* vertexTexture, Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const Fixture &selectedShape, Vertex &selectedVertex, Vertex &vertex, bool &moveVertex, SDL_FPoint &offset, bool moveImage,  bool zoomToMouse, const float &vertexRadius, SDL_FRect &image) {
    auto o = Calc_Offset(texture, vertexRadius, image);
    SDL_Color shapeFill = {200, 200, 200, 100};

    for (int k = 0; k < SIZE; ++k) {
      for (int i = 0; i < shapes[k].size(); ++i) {
        if (k == selectedShape.shape && i == selectedShape.indexPolygon)
          continue;
        Render_Shape(renderer, vertexTexture, texture, shapes, selectedShape, selectedVertex, vertex, moveVertex, offset, o, k, i, shapeFill, moveImage, zoomToMouse);
      }
    }

    //render selected shape
    if (selectedShape.shape != SIZE) {
      Render_Shape(renderer, vertexTexture, texture, shapes, selectedShape, selectedVertex, vertex, moveVertex, offset, o, selectedShape.shape, selectedShape.indexPolygon, shapeFill, moveImage, zoomToMouse);
    }
  }
  SDL_FPoint Vertex_To_Screen(Graphics::Image &texture, const SDL_FPoint &vertex, const Offsets &o, SDL_FPoint &offset, bool &moveImage, bool &moveVertex, bool &zoomToMouse) {
    SDL_FRect rect;
    rect.x = (vertex.x * texture.scale) + o.x;
    rect.y = (vertex.y * texture.scale) + o.y;
    if (moveImage || moveVertex || zoomToMouse) {
      rect.x -= (float) offset.x;
      rect.y -= (float) offset.y;
    }
    rect.x -= (float) texture.offset.x;
    rect.y -= (float) texture.offset.y;
    return {rect.x, rect.y};
  }
  Vertex Get_Vertex(Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const float &vertexRadius, SDL_FRect &image, const SDL_FRect &cursor, SDL_FPoint &offset, bool &moveImage, bool &zoomToMouse) {
    auto o = Calc_Offset(texture, vertexRadius, image);
    auto &shape = shapes;

    for (int k = 0; k < ::Shape::SIZE; ++k) {
      if (k == ::Shape::CIRCLE) {
        for (int i = 0; i < shape[k].size(); ++i) {
          auto vRect = Vertex_To_Rect(texture, shape[k][i].vertices[1], o, offset, shape[k][i].moving[1], moveImage, zoomToMouse);
          if (SDL_HasIntersectionF(&vRect, &cursor)) {
            shape[k][i].moving[1] = true;
            return {(::Shape::shape) k, i, 1};
          }
        }
      }
      else {
        for (int i = 0; i < shape[k].size(); ++i) {
          for (int j = 0; j < shape[k][i].vertices.size(); ++j) {
            auto vRect = Vertex_To_Rect(texture, shape[k][i].vertices[j], o, offset, shape[k][i].moving[j], moveImage, zoomToMouse);
            if (SDL_HasIntersectionF(&vRect, &cursor)) {
              shape[k][i].moving[j] = true;
              return {(::Shape::shape)k, i, j};
            }
          }
        }
      }
    }
    return {::Shape::SIZE, 0};
  }
  Vertex Move (std::array<std::vector<Shape>, SIZE> &shapes, const Fixture &selectedShape) {
    for (auto &&move: shapes[selectedShape.shape][selectedShape.indexPolygon].moving)
      move = true;
    return {selectedShape.shape, selectedShape.indexPolygon, -1};
  }
  Vertex Check_If_Selected(Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const float &vertexRadius, SDL_FRect &image, SDL_FPoint &offset, bool &moveImage, bool &zoomToMouse, const SDL_FRect &cursor, const Fixture &selectedShape) {
    if (selectedShape.shape != SIZE) {
      auto o = Calc_Offset(texture, vertexRadius, image);
      std::vector<SDL_FPoint> mouseVertexes;
      mouseVertexes.push_back({cursor.x, cursor.y});
      mouseVertexes.push_back({cursor.x + cursor.w, cursor.y});
      mouseVertexes.push_back({cursor.x + cursor.w, cursor.y + cursor.h});
      mouseVertexes.push_back({cursor.x, cursor.y + cursor.h});
      std::vector<SDL_FPoint> shapeVertexes;

      for (int j = 0; j < shapes[selectedShape.shape][selectedShape.indexPolygon].vertices.size(); ++j) {
        bool moving = shapes[selectedShape.shape][selectedShape.indexPolygon].moving[j];
        SDL_FPoint v = Vertex_To_Screen(texture, shapes[selectedShape.shape][selectedShape.indexPolygon].vertices[j], o, offset, moveImage, moving, zoomToMouse);
        shapeVertexes.emplace_back(v);
      }
      if (selectedShape.shape == CIRCLE) {
        if (Circle_Intersect(shapeVertexes[0].x, shapeVertexes[0].y, shapeVertexes[1].y - shapeVertexes[0].y, cursor))
          return Move(shapes, selectedShape);
      }
      else if (selectedShape.shape == POLYGON) {
        if (Point_In_Polygon(mouseVertexes, shapeVertexes))
          return Move(shapes, selectedShape);
      }
      else if (selectedShape.shape == AABB || selectedShape.shape == LINE) {
        if (PolygonOverlap_SAT(mouseVertexes, shapeVertexes))
          return Move(shapes, selectedShape);
      }
    }
    return {::Shape::SIZE, 0};
  }




  Vertex Get_Shape(Graphics::Image &texture, std::array<std::vector<Shape>, SIZE> &shapes, const float &vertexRadius, SDL_FRect &image, SDL_FPoint &offset, bool &moveImage, bool &zoomToMouse, const SDL_FRect &cursor, const Fixture &selectedShape) {

    //check if I already have a shape first, return it if I still have it selected

    auto shape = Check_If_Selected(texture, shapes, vertexRadius, image, offset, moveImage, zoomToMouse, cursor, selectedShape);
    if (shape.shape != SIZE)
      return shape;

    for (int k = 0; k < SIZE; ++k) {
      for (int i = (int)shapes[k].size() - 1; i >= 0; --i) {
        Fixture selected;
        selected.shape = (::Shape::shape)k;
        selected.indexPolygon= i;
        shape = Check_If_Selected(texture, shapes, vertexRadius, image, offset, moveImage, zoomToMouse, cursor, selected);
        if (shape.shape != SIZE)
          return shape;
      }
    }
    return {SIZE, 0};
  }

}