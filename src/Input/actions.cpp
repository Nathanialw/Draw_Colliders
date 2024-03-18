//
// Created by nathanial on 17/03/24.
//

#include "actions.h"
#include "../App/core.h"
#include "../App/Settings/save.h"
#include "../Graphics/text.h"

namespace Action {
  bool Delete_Shape(App::App &app) {
    switch (app.selectedShape.shape) {
      case Graphics::SIZE:
        return false;

      case Graphics::POLYGON: {
        auto &polygon = app.interface.center.polygons;
        //              polygon[app.selectedShape.indexPolygon].vertexes.clear();
        //              polygon[app.selectedShape.indexPolygon].moving.clear();
        polygon.erase(polygon.begin() + app.selectedShape.indexPolygon, polygon.begin() + app.selectedShape.indexPolygon + 1);
        polygon.shrink_to_fit();
        app.selectedShape.shape = Graphics::SIZE;
        app.selectedShape.indexPolygon = 0;
        app.selectedVertex.shape = Graphics::SIZE;
        app.selectedVertex.indexPolygon = 0;
        app.selectedVertex.indexVertex = 0;
        return true;
      }
      case Graphics::AABB: {
        auto &rect = app.interface.center.rects;
        rect.erase(rect.begin() + app.selectedShape.indexPolygon, rect.begin() + app.selectedShape.indexPolygon + 1);
        rect.shrink_to_fit();
        app.selectedShape.shape = Graphics::SIZE;
        app.selectedShape.indexPolygon = 0;
        return true;
      }
      case Graphics::CIRCLE: {
        auto &circles = app.interface.center.circles;
        circles.erase(circles.begin() + app.selectedShape.indexPolygon, circles.begin() + app.selectedShape.indexPolygon + 1);
        circles.shrink_to_fit();
        app.selectedShape.shape = Graphics::SIZE;
        app.selectedShape.indexPolygon = 0;
        return true;
      }
      case Graphics::LINE: {
        auto &line = app.interface.center.lineSegments;
        line.erase(line.begin() + app.selectedShape.indexPolygon, line.begin() + app.selectedShape.indexPolygon + 1);
        line.shrink_to_fit();
        app.selectedShape.shape = Graphics::SIZE;
        app.selectedShape.indexPolygon = 0;
        return true;
      }
      case Graphics::POINT: {
        auto &point = app.interface.center.points;
        point.erase(point.begin() + app.selectedShape.indexPolygon, point.begin() + app.selectedShape.indexPolygon + 1);
        point.shrink_to_fit();
        app.selectedShape.shape = Graphics::SIZE;
        app.selectedShape.indexPolygon = 0;
        return true;
      }
    }
    return false;
  }

  bool Delete_Vertex(App::App &app) {
    //delete vertex
    if (app.selectedVertex.shape == Graphics::POLYGON) {
      auto &polygon = app.interface.center.polygons[app.selectedVertex.indexPolygon];
      polygon.vertexes.erase(polygon.vertexes.begin() + app.selectedVertex.indexVertex, polygon.vertexes.begin() + app.selectedVertex.indexVertex + 1);
      polygon.moving.erase(polygon.moving.begin() + app.selectedVertex.indexVertex, polygon.moving.begin() + app.selectedVertex.indexVertex + 1);
      polygon.vertexes.shrink_to_fit();
      polygon.moving.shrink_to_fit();
      app.selectedVertex.shape = Graphics::SIZE;
      app.selectedVertex.indexVertex = 0;
      app.selectedVertex.indexPolygon = 0;
      return true;
    }
    return false;
  }

  int Get_Vertex_Position(const std::vector<SDL_FPoint> &vertexes, const SDL_FPoint &point) {
    float distance = MAXFLOAT;
    int index = 0;
//    check each line segment
    for (int i = 0; i < vertexes.size() - 1; ++i) {
      float x = distanceToLineSegment(point, vertexes[i], vertexes[i + 1]);
      if (x < distance) {
        distance = x;
        index = i + 1;
      }
    }
    float x = distanceToLineSegment(point, vertexes[vertexes.size() - 1], vertexes[0]);
    if (x < distance)
      index = vertexes.size() + 1;
    return index;
  }

  bool Add_Vertex(App::App &app) {
    //add vertex
    if (app.selectedShape.shape == Graphics::POLYGON) {
      i2 m{};
      SDL_GetMouseState(&m.x, &m.y);
      SDL_FPoint pos = {(float)m.x, (float)m.y};
      pos = Offset_From_Image_Center(app, pos);
      //instead of push back, insert vertex between the 2 closest vertices to the mouse
      auto &vertices = app.interface.center.polygons[app.selectedShape.indexPolygon].vertexes;
      int index = Get_Vertex_Position(vertices, pos);

      if (index > vertices.size())
        vertices.push_back({pos.x,pos.y});
      else
      vertices.insert(vertices.begin() + index, {pos.x,pos.y});
      auto &moving = app.interface.center.polygons[app.selectedShape.indexPolygon].moving;
      if (index > moving.size())
        moving.push_back(false);
      else
      moving.insert(moving.begin() + index, false);
      return true;
    }
    return false;
  }

  bool Add_Vertex_Center(App::App &app) {
    if (app.selectedShape.shape == Graphics::POLYGON) {
      app.interface.center.polygons[app.selectedShape.indexPolygon].vertexes.push_back({0.0f, 0.0f});
      app.interface.center.polygons[app.selectedShape.indexPolygon].moving.push_back(false);
      return true;
    }
    return false;
  }

  bool Add_Image(App::App &app) {
    auto imageImport = Graphics::Load_Image(app.context);
    if (imageImport.texture) {
      Data::Center image;
      image.texture.texture = imageImport.texture;
      image.index = app.interface.left.images.size();

      app.interface.left.images.emplace_back(image);
      app.interface.left.imageNameStr.emplace_back(
          Text::Get_File_Name(imageImport.fileName));
      app.interface.left.imagePathStr.emplace_back(imageImport.fileName);

      // load the image to the center if there is no image
      if (!app.interface.center.texture.texture)
        app.interface.center = image;
      return true;
    }
    return false;
  }

  bool New_Project(App::App &app) {
    App::New(app);
    return true;
  }

  bool Close_Project(App::App &app) {
    App::New(app);
    return true;
  }

  bool Quit_Application(App::App &app) {
    app.running = !app.running;
    return true;
  }

  bool Save(App::App &app) {
    Save::State(app);
    Serialise::Datafile::Write(app.datafile, "save.txt");
    return true;
  }

  bool Load(App::App &app) {
    Save::Load(app);
    return true;
  }













}