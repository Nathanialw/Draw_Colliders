//
// Created by nathanial on 17/03/24.
//

#include "actions.h"
#include "../App/Settings/save.h"
#include "../Graphics/text.h"
#include "../Output/json.h"
#include "../Output/xml.h"
#include "../Output/sqlite.h"
#include "../UI/scroll_bar.h"
#include "iostream"

namespace Action {
  bool Delete_Shape_(App::App &app, const Shape::shape &shap, const int &shapeIndex) {
    auto &shape = app.interface.center.shapes[shap];
    shape.erase(shape.begin() + shapeIndex, shape.begin() + shapeIndex + 1);
    shape.shrink_to_fit();
    auto &shapeList = app.interface.shapeList.shapeList[shap];
    shapeList.erase(shapeList.begin() + shapeIndex + 1, shapeList.begin() + shapeIndex + 2);
    shapeList.shrink_to_fit();

    if (shap == app.selectedShape.shape && shapeIndex == app.selectedShape.indexPolygon) {
      app.selectedShape.shape = Shape::SIZE;
      app.selectedShape.indexPolygon = 0;
    }
    return true;
  }

  bool Delete_Shape_(App::App &app) {
    if (app.selectedShape.shape != Shape::SIZE) {
      Delete_Shape_(app, app.selectedShape.shape, app.selectedShape.indexPolygon );
      app.vertex.shape = Shape::SIZE;
      app.vertex.indexPolygon = 0;
      app.vertex.indexVertex = 0;
      return true;
    }
    return false;
  }

  bool Delete_Vertex_(App::App &app) {
    //delete vertex
    if (app.selectedVertex.shape == Shape::POLYGON) {
      auto &polygon = app.interface.center.shapes[Shape::POLYGON][app.selectedVertex.indexPolygon];
      if (polygon.vertices.size() < 4)
        Delete_Shape_(app, app.selectedVertex.shape, app.selectedVertex.indexPolygon);
      else {
        polygon.vertices.erase(polygon.vertices.begin() + app.selectedVertex.indexVertex, polygon.vertices.begin() + app.selectedVertex.indexVertex + 1);
        polygon.moving.erase(polygon.moving.begin() + app.selectedVertex.indexVertex, polygon.moving.begin() + app.selectedVertex.indexVertex + 1);
        polygon.vertices.shrink_to_fit();
        polygon.moving.shrink_to_fit();
      }
      app.selectedVertex.shape = Shape::SIZE;
      app.selectedVertex.indexVertex = 0;
      app.selectedVertex.indexPolygon = 0;
      app.vertex.shape = Shape::SIZE;
      app.vertex.indexPolygon = 0;
      app.vertex.indexVertex = 0;
      return true;
    }
    return false;
  }

  int Get_Vertex_Position_(const std::vector<SDL_FPoint> &vertexes, const SDL_FPoint &point) {
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

  bool Add_Vertex_(App::App &app) {
    //add vertex
    if (app.selectedShape.shape == Shape::POLYGON) {
      i2 m{};
      SDL_GetMouseState(&m.x, &m.y);
      SDL_FPoint pos = {(float)m.x, (float)m.y};
      pos = Offset_From_Image_Center(app, pos);
      //instead of push back, insert vertex between the 2 closest vertices to the mouse
      auto &vertices = app.interface.center.shapes[Shape::POLYGON][app.selectedShape.indexPolygon].vertices;
      int index = Get_Vertex_Position_(vertices, pos);

      if (index > vertices.size())
        vertices.push_back({pos.x,pos.y});
      else
        vertices.insert(vertices.begin() + index, {pos.x,pos.y});
      auto &moving = app.interface.center.shapes[Shape::POLYGON][app.selectedShape.indexPolygon].moving;
      if (index > moving.size())
        moving.push_back(false);
      else
        moving.insert(moving.begin() + index, false);
      app.selectedVertex.shape = Shape::POLYGON;
      app.selectedVertex.indexPolygon = app.selectedShape.indexPolygon;
      app.selectedVertex.indexVertex = index;
      return true;
    }
    return false;
  }

  bool Add_Vertex_Center_(App::App &app) {
    if (app.selectedShape.shape == Shape::POLYGON) {
      auto &vertices = app.interface.center.shapes[Shape::POLYGON][app.selectedShape.indexPolygon].vertices;
      SDL_FPoint pos = {0.0f, 0.0f};
      int index = Get_Vertex_Position_(vertices, pos);

      if (index > vertices.size())
        vertices.push_back({pos.x,pos.y});
      else
        vertices.insert(vertices.begin() + index, {pos.x,pos.y});
      auto &moving = app.interface.center.shapes[Shape::POLYGON][app.selectedShape.indexPolygon].moving;
      if (index > moving.size())
        moving.push_back(false);
      else
        moving.insert(moving.begin() + index, false);

      app.selectedVertex.shape = Shape::POLYGON;
      app.selectedVertex.indexPolygon = app.selectedShape.indexPolygon;
      app.selectedVertex.indexVertex = index;
      return true;
    }
    return false;
  }

  void Import_Image_(Data::Center &center, Data::Left &left, const Graphics::Image_Import &imageImport) {
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

  bool Add_Image_(App::App &app) {
    auto imageImport = Graphics::Load_Image(app.context);
    if (imageImport.texture) {
      Import_Image_(app.interface.center, app.interface.left, imageImport);

      Scroll_Bar::Update(app, app.panel.mainPanel.left.scroll, app.uiPanels.scrollBarLeftHeight, app.interface.left.images.size(), app.panel.mainPanel.left.body.w);
      return true;
    }
    return false;
  }

  bool Add_Images_(App::App &app) {
    std::vector<Graphics::Image_Import> imagesImport = Graphics::Load_Images(app.context);
    if (!imagesImport.empty()) {

      for (const auto &imageImport : imagesImport) {
        Import_Image_(app.interface.center, app.interface.left, imageImport);
      }

      Scroll_Bar::Update(app, app.panel.mainPanel.left.scroll, app.uiPanels.scrollBarLeftHeight, app.interface.left.images.size(), app.panel.mainPanel.left.body.w);
      return true;
    }
    return false;
  }

  bool Remove_Image_(App::App &app) {
    if (app.interface.left.images.size() > app.imageIndex) {

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

      if (app.imageIndex > app.interface.left.images.size() - 1)
        app.imageIndex -= 1;
      app.interface.left.selected = app.imageIndex;
      if (!app.interface.left.images.empty())
        app.interface.center = app.interface.left.images[app.imageIndex];

      Scroll_Bar::Update(app, app.panel.mainPanel.left.scroll, app.uiPanels.scrollBarLeftHeight, app.interface.left.images.size(), app.panel.mainPanel.left.body.w);
      return true;
    }
    return false;
  }


  int Delete_Shape(App::App &app){
    if (Delete_Shape_(app))
      return 1;
    return 0;
  };
  int Delete_Vertex(App::App &app){
    if (Delete_Vertex_(app))
      return 1;
    return 0;
  };
  int Add_Vertex(App::App &app){
    if (Add_Vertex_(app))
      return 1;
    return 0;
  };
  int Add_Vertex_Center(App::App &app){
    if (Add_Vertex_Center_(app))
      return 1;
    return 0;
  };
  int Add_Image(App::App &app){
    if (Add_Image_(app))
      return 1;
    return 0;
  };
  int Add_Images(App::App &app){
    if (Add_Images_(app))
      return 1;
    return 0;
  };
  int Remove_Image(App::App &app){
    if (Remove_Image_(app))
      return 1;
    return 0;
  };
  int New_Project(App::App &app){
    App::New(app);
    return 1;
  };
  int Open_Project(App::App &app){
    Save::Load_As(app);
    return 1;
  };
  int Close_Project(App::App &app){
    App::New(app);
    return 1;
  };
  int Quit_Application(App::App &app){
    app.running = !app.running;
    return 1;
  };
  int Publish(App::App &app){
    SQLite::Publish(app);
    return 1;
  };
  int Publish_As(App::App &app){
    SQLite::Publish(app);
    return 1;
  };
  int Save(App::App &app){
    if (!app.saveName.empty())
      Save::Save(app, app.saveName);
    else if (Save::Save_As(app))
      return 1;
    return 0;
  };
  int Save_As(App::App &app){
    if (Save::Save_As(app))
      return 1;
    return 0;
  };
  int Load(App::App &app){
    Save::Load_As(app);
    return 1;
  };



  typedef Shape::Shape (*CREATE_SHAPE)();

  bool Add_Shape(App::App &app, const Shape::shape &shape, const CREATE_SHAPE &Create) {
    if (app.interface.center.texture.texture) {
      auto &shapeList = app.interface.shapeList.shapeList[shape];
      for (int j = 0; j < app.interface.center.shapes[shape].size(); ++j) {
        if (atoi(shapeList[j + 1].c_str()) != j) {
          shapeList.insert(shapeList.begin() + j + 1, std::to_string(j));
          app.interface.center.shapes[shape].insert(app.interface.center.shapes[shape].begin() + j, Create());
          return true;
        }
      }
      shapeList.emplace_back(std::to_string(app.interface.center.shapes[shape].size()));
      app.interface.center.shapes[shape].emplace_back(Create());
      return true;
    }
    return false;
  }

  int Create_Circle(App::App &app) {
    Add_Shape(app, Shape::CIRCLE, Circle::Create);
    return 0;
  }
  int Create_Point(App::App &app) {
    Add_Shape(app, Shape::POINT, Point::Create);
    return 1;
  }
  int Create_Polygon(App::App &app) {
    Add_Shape(app, Shape::POLYGON, Polygon::Create);
    return 2;
  }
  int Create_Rect(App::App &app) {
    Add_Shape(app, Shape::AABB, AABB::Create);
    return 3;
  }
  int Create_Line(App::App &app) {
    Add_Shape(app, Shape::LINE, Line_Segment::Create);
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
  int UNIMPLEMENTED(App::App &app) {
    std::cout << "button unassigned" << std::endl;
    return 5;
  }







}