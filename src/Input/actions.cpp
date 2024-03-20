//
// Created by nathanial on 17/03/24.
//

#include "actions.h"
#include "../App/Settings/save.h"
#include "../Graphics/text.h"
#include "../Output/json.h"
#include "../Output/xml.h"
#include "../Output/sqlite.h"

namespace Action {
  bool Delete_Shape(App::App &app, const Graphics::Shape &shap, const int &shapeIndex) {
    auto &shape = app.interface.center.shapes[shap];
    shape.erase(shape.begin() + shapeIndex, shape.begin() + shapeIndex + 1);
    shape.shrink_to_fit();
    auto &shapeList = app.interface.shapeList.shapeList[shap];
    shapeList.erase(shapeList.begin() + shapeIndex + 1, shapeList.begin() + shapeIndex + 2);
    shapeList.shrink_to_fit();

    if (shap == app.selectedShape.shape && shapeIndex == app.selectedShape.indexPolygon) {
      app.selectedShape.shape = Graphics::SIZE;
      app.selectedShape.indexPolygon = 0;
    }
    return true;
  }

  bool Delete_Shape(App::App &app) {
    if (app.selectedShape.shape != Graphics::SIZE) {
      Delete_Shape(app, app.selectedShape.shape, app.selectedShape.indexPolygon );
      app.vertex.shape = Graphics::SIZE;
      app.vertex.indexPolygon = 0;
      app.vertex.indexVertex = 0;
      return true;
    }
    return false;
  }

  bool Delete_Vertex(App::App &app) {
    //delete vertex
    if (app.selectedVertex.shape == Graphics::POLYGON) {
      auto &polygon = app.interface.center.shapes[Graphics::POLYGON][app.selectedVertex.indexPolygon];
      if (polygon.vertices.size() < 4)
        Delete_Shape(app, app.selectedVertex.shape, app.selectedVertex.indexPolygon);
      else {
        polygon.vertices.erase(polygon.vertices.begin() + app.selectedVertex.indexVertex, polygon.vertices.begin() + app.selectedVertex.indexVertex + 1);
        polygon.moving.erase(polygon.moving.begin() + app.selectedVertex.indexVertex, polygon.moving.begin() + app.selectedVertex.indexVertex + 1);
        polygon.vertices.shrink_to_fit();
        polygon.moving.shrink_to_fit();
      }
//      SDL_DestroyTexture(app.texture.shapes[app.interface.center.index][Graphics::POLYGON][app.selectedVertex.indexPolygon]);
      app.selectedVertex.shape = Graphics::SIZE;
      app.selectedVertex.indexVertex = 0;
      app.selectedVertex.indexPolygon = 0;
      app.vertex.shape = Graphics::SIZE;
      app.vertex.indexPolygon = 0;
      app.vertex.indexVertex = 0;
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
      auto &vertices = app.interface.center.shapes[Graphics::POLYGON][app.selectedShape.indexPolygon].vertices;
      int index = Get_Vertex_Position(vertices, pos);

      if (index > vertices.size())
        vertices.push_back({pos.x,pos.y});
      else
        vertices.insert(vertices.begin() + index, {pos.x,pos.y});
      auto &moving = app.interface.center.shapes[Graphics::POLYGON][app.selectedShape.indexPolygon].moving;
      if (index > moving.size())
        moving.push_back(false);
      else
        moving.insert(moving.begin() + index, false);
//      SDL_DestroyTexture(app.texture.shapes[app.interface.center.index][Graphics::POLYGON][app.selectedShape.indexPolygon]);
      app.selectedVertex.shape = Graphics::POLYGON;
      app.selectedVertex.indexPolygon = app.selectedShape.indexPolygon;
      app.selectedVertex.indexVertex = index;
      return true;
    }
    return false;
  }

  bool Add_Vertex_Center(App::App &app) {
    if (app.selectedShape.shape == Graphics::POLYGON) {
      auto &vertices = app.interface.center.shapes[Graphics::POLYGON][app.selectedShape.indexPolygon].vertices;
      SDL_FPoint pos = {0.0f, 0.0f};
      int index = Get_Vertex_Position(vertices, pos);

      if (index > vertices.size())
        vertices.push_back({pos.x,pos.y});
      else
        vertices.insert(vertices.begin() + index, {pos.x,pos.y});
      auto &moving = app.interface.center.shapes[Graphics::POLYGON][app.selectedShape.indexPolygon].moving;
      if (index > moving.size())
        moving.push_back(false);
      else
        moving.insert(moving.begin() + index, false);

//      SDL_DestroyTexture(app.texture.shapes[app.interface.center.index][Graphics::POLYGON][app.selectedShape.indexPolygon]);
      app.selectedVertex.shape = Graphics::POLYGON;
      app.selectedVertex.indexPolygon = app.selectedShape.indexPolygon;
      app.selectedVertex.indexVertex = index;
      return true;
    }
    return false;
  }

  void Import_Image(Data::Center &center, Data::Left &left, const Graphics::Image_Import &imageImport) {
    Data::Center image;
    image.texture.texture = imageImport.texture;
    image.index = left.images.size();

    left.images.emplace_back(image);
    left.imageNameStr.emplace_back(Text::Get_File_Name(imageImport.fileName));
    left.imagePathStr.emplace_back(imageImport.fileName);

    // load the image to the center if there is no image
    if (!center.texture.texture)
      center = image;
  }

  bool Add_Image(App::App &app) {
    auto imageImport = Graphics::Load_Image(app.context);
    if (imageImport.texture) {
      Import_Image(app.interface.center, app.interface.left, imageImport);
      return true;
    }
    return false;
  }

  bool Add_Images(App::App &app) {
    std::vector<Graphics::Image_Import> imagesImport = Graphics::Load_Images(app.context);
    if (!imagesImport.empty()) {
      for (const auto &imageImport : imagesImport) {
        Import_Image(app.interface.center, app.interface.left, imageImport);
      }
      return true;
    }
    return false;
  }

  bool Remove_Image(App::App &app) {
    if (app.interface.left.images.size() > app.imageIndex) {

      for (auto &shapes : app.texture.shapes[app.imageIndex]) {
        for (auto &shape :shapes) {
          SDL_DestroyTexture(shape);
        }
      }
      app.texture.shapes.erase(app.texture.shapes.begin() + app.imageIndex);

      SDL_DestroyTexture(app.interface.left.images[app.imageIndex].texture.texture);
      app.interface.left.images.erase(app.interface.left.images.begin() + app.imageIndex);
//      app.interface.left.imageNames.erase(app.interface.left.imageNames.begin() + app.imageIndex);
      app.interface.left.imagePathStr.erase(app.interface.left.imagePathStr.begin() + app.imageIndex);
      app.interface.left.imageNameStr.erase(app.interface.left.imageNameStr.begin() + app.imageIndex);

      if (app.interface.center.index == app.imageIndex) {
        app.interface.center = {};
      }

      for (int i = app.imageIndex; i < app.interface.left.images.size(); ++i) {
        app.interface.left.images[i].index--;
      }
      app.imageIndex = 0;
      app.interface.left.selected = 0;
      if (!app.interface.left.images.empty())
        app.interface.center = app.interface.left.images[0];

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

  bool Open_Project(App::App &app) {
    Save::Load_As(app);
    return true;
  }

  bool Quit_Application(App::App &app) {
    app.running = !app.running;
    return true;
  }

  bool Save(App::App &app) {
    if (!app.saveName.empty())
      Save::Save(app, app.saveName);
    else if (Save::Save_As(app))
      return true;
    return false;
  }

  bool Save_As(App::App &app) {
    if (Save::Save_As(app))
      return true;
    return false;
  }

  bool Load(App::App &app) {
    Save::Load_As(app);
    return true;
  }

  bool PublishJSON(App::App &app) {
    SQLite::Publish(app);
    return true;
  }

  bool PublishXML(App::App &app) {
    SQLite::Publish(app);
    return true;
  }













}