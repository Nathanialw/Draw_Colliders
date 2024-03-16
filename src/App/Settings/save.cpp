//
// Created by nathanial on 3/13/24.
//

#include "serialise.h"
#include "save.h"
#include "../../Graphics/text.h"
#include "../../Graphics/graphics.h"

namespace Save {

  void State(App::App &app) {
    std::array<std::string, 5> shapeStr = {"point", "circle", "lineSegment", "aabb", "polygon"};
    //resets the in memory data to save the file
    Serialise::Datafile datafile;
    app.datafile = datafile;

    app.datafile["header"]["name"].Set_String("name of save file");
    //maybe more than 1
    app.datafile["header"]["folder"].Set_String("path to folder with images");
    app.datafile["header"]["image_count"].Set_Int(app.interface.left.imagePathStr.size());

    for (int i = 0; i < app.interface.left.imagePathStr.size(); ++i) {
      app.datafile["image[" + std::to_string(i) + "]"]["name"].Set_String(app.interface.left.imageNameStr[i]);
      app.datafile["image[" + std::to_string(i) + "]"]["path"].Set_String(app.interface.left.imagePathStr[i]);
      app.datafile["image[" + std::to_string(i) + "]"]["scale"].Set_Real(app.interface.left.images[i].texture.scale);
      int k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[0]]["shape_count"].Set_Int(app.interface.left.images[i].points.size());
      for (const auto &point: app.interface.left.images[i].points) {
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[0]][std::to_string(k)]["x"].Set_Real(point.vertex.x);
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[0]][std::to_string(k)]["y"].Set_Real(point.vertex.y);
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]]["shape_count"].Set_Int(app.interface.left.images[i].circles.size());
      for (const auto &circle: app.interface.left.images[i].circles) {
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]][std::to_string(k)]["x"].Set_Real(circle.vertexes[0].x);
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]][std::to_string(k)]["y"].Set_Real(circle.vertexes[0].y);
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]][std::to_string(k)]["r"].Set_Real(circle.r);
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[2]]["shape_count"].Set_Int(app.interface.left.images[i].lineSegments.size());
      for (const auto &line: app.interface.left.images[i].lineSegments) {
        int vertexCount = 0;
        for (const auto &vertex : line.vertexes) {
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[2]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[2]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[3]]["shape_count"].Set_Int(app.interface.left.images[i].rects.size());
      for (const auto &aabb: app.interface.left.images[i].rects) {
        int vertexCount = 0;
        for (const auto &vertex : aabb.vertexes) {
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[3]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[3]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[4]]["shape_count"].Set_Int(app.interface.left.images[i].polygons.size());
      for (const auto &polygon: app.interface.left.images[i].polygons) {
        int vertexCount = 0;
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[4]][std::to_string(k)]["vertex_count"].Set_Int(polygon.vertexes.size());
        for (const auto &vertex : polygon.vertexes) {
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[4]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[4]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
    }
  }



  void Load (App::App &app) {
    app.datafile.Read(app.datafile, "save.txt");

    std::array<std::string, 5> shapeStr = {"point", "circle", "lineSegment", "aabb", "polygon"};
    //minus the header
    auto num = app.datafile["header"]["image_count"].Get_Int();
    //clear values
    Data::Left newLeft;
    app.interface.left = newLeft;
    bool setCenter = true;

    for (int i = 0; i < num; ++i) {
      if (app.interface.left.imageNameStr.size() <= i)
        app.interface.left.imageNameStr.resize(i + 1);
      if (app.interface.left.imagePathStr.size() <= i)
        app.interface.left.imagePathStr.resize(i + 1);

      auto &image = app.datafile["image[" + std::to_string(i) + "]"];
      std::string name = image["name"].Get_String();
      std::string path = image["path"].Get_String();

      //load
      SDL_Texture* texture = Graphics::Load_Texture(app.context, path);
      if (texture) {
        Data::Center imageLoaded;
        imageLoaded.texture.texture = texture;
        imageLoaded.index = app.interface.left.images.size();
        app.interface.left.images.emplace_back(imageLoaded);
        app.interface.left.images[i].texture.scale = app.datafile["image[" + std::to_string(i) + "]"]["scale"].Get_Real();
        app.interface.left.imageNameStr[i] = name;
        app.interface.left.imagePathStr[i] = path;

//        load the image to the center if there is no image

      }

      auto &shapes = app.interface.left.images[i];
      //need to save the number of each shape
      int shape_count = image[shapeStr[0]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.points.size() <= shape_count)
        shapes.points.resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        //need to save the number of each vertex in the shape
        shapes.points[l].vertex.x = image[shapeStr[0]][std::to_string(l)]["x"].Get_Real();
        shapes.points[l].vertex.y = image[shapeStr[0]][std::to_string(l)]["y"].Get_Real();
      }

      shape_count = image[shapeStr[1]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.circles.size() <= shape_count)
        shapes.circles.resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        //need to save the number of each vertex in the shape
        shapes.circles[l].vertexes[0].x = image[shapeStr[1]][std::to_string(l)]["x"].Get_Real();
        shapes.circles[l].vertexes[0].y = image[shapeStr[1]][std::to_string(l)]["y"].Get_Real();
        shapes.circles[l].r =             image[shapeStr[1]][std::to_string(l)]["r"].Get_Real();
        shapes.circles[l].vertexes[1].x = image[shapeStr[1]][std::to_string(l)]["x"].Get_Real();
        shapes.circles[l].vertexes[1].y = image[shapeStr[1]][std::to_string(l)]["y"].Get_Real() + shapes.circles[l].r;
      }

      shape_count = image[shapeStr[2]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.lineSegments.size() <= shape_count)
        shapes.lineSegments.resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.lineSegments[l].vertexes[0].x = image[shapeStr[2]][std::to_string(l)]["0"]["x"].Get_Real();
        shapes.lineSegments[l].vertexes[0].y = image[shapeStr[2]][std::to_string(l)]["0"]["y"].Get_Real();
        shapes.lineSegments[l].vertexes[1].x = image[shapeStr[2]][std::to_string(l)]["1"]["x"].Get_Real();
        shapes.lineSegments[l].vertexes[1].y = image[shapeStr[2]][std::to_string(l)]["1"]["y"].Get_Real();
      }

      shape_count = image[shapeStr[3]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.rects.size() <= shape_count)
        shapes.rects.resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.rects[l].vertexes[0].x = image[shapeStr[3]][std::to_string(l)]["0"]["x"].Get_Real();
        shapes.rects[l].vertexes[0].y = image[shapeStr[3]][std::to_string(l)]["0"]["y"].Get_Real();
        shapes.rects[l].vertexes[1].x = image[shapeStr[3]][std::to_string(l)]["1"]["x"].Get_Real();
        shapes.rects[l].vertexes[1].y = image[shapeStr[3]][std::to_string(l)]["1"]["y"].Get_Real();
        shapes.rects[l].vertexes[2].x = image[shapeStr[3]][std::to_string(l)]["2"]["x"].Get_Real();
        shapes.rects[l].vertexes[2].y = image[shapeStr[3]][std::to_string(l)]["2"]["y"].Get_Real();
        shapes.rects[l].vertexes[3].x = image[shapeStr[3]][std::to_string(l)]["3"]["x"].Get_Real();
        shapes.rects[l].vertexes[3].y = image[shapeStr[3]][std::to_string(l)]["3"]["y"].Get_Real();
      }

      shape_count = image[shapeStr[4]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.polygons.size() <= shape_count)
        shapes.polygons.resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        int vertex_count = image[shapeStr[4]][std::to_string(l)]["vertex_count"].Get_Int();
//
        if (vertex_count != 0 && shapes.polygons[l].vertexes.size() <= vertex_count) {
          shapes.polygons[l].vertexes.resize(vertex_count);
          shapes.polygons[l].moving.resize(vertex_count);
        }

        for (int j = 0; j < vertex_count; ++j) {
          shapes.polygons[l].vertexes[j].x = image[shapeStr[4]][std::to_string(l)][std::to_string(j)]["x"].Get_Real();
          shapes.polygons[l].vertexes[j].y = image[shapeStr[4]][std::to_string(l)][std::to_string(j)]["y"].Get_Real();
          shapes.polygons[l].moving[j] = false;
        }
      }

      if (setCenter) {
        app.interface.center = app.interface.left.images[0];
        setCenter = false;
      }
    }
  }
}