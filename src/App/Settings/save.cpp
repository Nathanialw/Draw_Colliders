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
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[0]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::POINT].size());
      for (const auto &point: app.interface.left.images[i].shapes[Graphics::POINT]) {
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[0]][std::to_string(k)]["x"].Set_Real(point.vertices[0].x);
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[0]][std::to_string(k)]["y"].Set_Real(point.vertices[0].y);
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::CIRCLE].size());
      for (const auto &circle: app.interface.left.images[i].shapes[Graphics::CIRCLE]) {
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]][std::to_string(k)]["x"].Set_Real(circle.vertices[0].x);
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]][std::to_string(k)]["y"].Set_Real(circle.vertices[0].y);
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[1]][std::to_string(k)]["r"].Set_Real(circle.vertices[1].y - circle.vertices[0].y);
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[2]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::LINE].size());
      for (const auto &line: app.interface.left.images[i].shapes[Graphics::LINE]) {
        int vertexCount = 0;
        for (const auto &vertex : line.vertices) {
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[2]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[2]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[3]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::AABB].size());
      for (const auto &aabb: app.interface.left.images[i].shapes[Graphics::AABB]) {
        int vertexCount = 0;
        for (const auto &vertex : aabb.vertices) {
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[3]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          app.datafile["image[" + std::to_string(i) + "]"][shapeStr[3]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
      k = 0;
      app.datafile["image[" + std::to_string(i) + "]"][shapeStr[4]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::POLYGON].size());
      for (const auto &polygon: app.interface.left.images[i].shapes[Graphics::POLYGON]) {
        int vertexCount = 0;
        app.datafile["image[" + std::to_string(i) + "]"][shapeStr[4]][std::to_string(k)]["vertex_count"].Set_Int(polygon.vertices.size());
        for (const auto &vertex : polygon.vertices) {
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
      if (shape_count != 0 && shapes.shapes[Graphics::POINT].size() <= shape_count)
        shapes.shapes[Graphics::POINT].resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.shapes[Graphics::POINT][l] = Point::Create((float)image[shapeStr[0]][std::to_string(l)]["x"].Get_Real(), (float)image[shapeStr[0]][std::to_string(l)]["y"].Get_Real());
      }

      shape_count = image[shapeStr[1]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::CIRCLE].size() <= shape_count)
        shapes.shapes[Graphics::CIRCLE].resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.shapes[Graphics::CIRCLE][l] = Circle::Create(image[shapeStr[1]][std::to_string(l)]["x"].Get_Real(), image[shapeStr[1]][std::to_string(l)]["y"].Get_Real(), image[shapeStr[1]][std::to_string(l)]["y"].Get_Real() + image[shapeStr[1]][std::to_string(l)]["r"].Get_Real());
      }

      shape_count = image[shapeStr[2]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::LINE].size() <= shape_count)
        shapes.shapes[Graphics::LINE].resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.shapes[Graphics::LINE][l] = Line_Segment::Create(image[shapeStr[2]][std::to_string(l)]["0"]["x"].Get_Real(), image[shapeStr[2]][std::to_string(l)]["0"]["y"].Get_Real(), image[shapeStr[2]][std::to_string(l)]["1"]["x"].Get_Real(), image[shapeStr[2]][std::to_string(l)]["1"]["y"].Get_Real());
      }

      shape_count = image[shapeStr[3]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::AABB].size() <= shape_count)
        shapes.shapes[Graphics::AABB].resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.shapes[Graphics::AABB][l].vertices.resize(4);
        shapes.shapes[Graphics::AABB][l].moving.resize(4);
        for (int j = 0; j < 4; ++j) {
          shapes.shapes[Graphics::AABB][l].vertices[j] = {(float)image[shapeStr[3]][std::to_string(l)][std::to_string(j)]["x"].Get_Real(), (float)image[shapeStr[3]][std::to_string(l)][std::to_string(j)]["y"].Get_Real()};
          shapes.shapes[Graphics::AABB][l].moving[j] = false;
        }
      }

      shape_count = image[shapeStr[4]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::POLYGON].size() <= shape_count)
        shapes.shapes[Graphics::POLYGON].resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        int vertex_count = image[shapeStr[4]][std::to_string(l)]["vertex_count"].Get_Int();
//
        if (vertex_count != 0 && shapes.shapes[Graphics::POLYGON][l].vertices.size() <= vertex_count) {
          shapes.shapes[Graphics::POLYGON][l].vertices.resize(vertex_count);
          shapes.shapes[Graphics::POLYGON][l].moving.resize(vertex_count);
        }

        for (int j = 0; j < vertex_count; ++j) {
          shapes.shapes[Graphics::POLYGON][l].vertices[j].x = image[shapeStr[4]][std::to_string(l)][std::to_string(j)]["x"].Get_Real();
          shapes.shapes[Graphics::POLYGON][l].vertices[j].y = image[shapeStr[4]][std::to_string(l)][std::to_string(j)]["y"].Get_Real();
          shapes.shapes[Graphics::POLYGON][l].moving[j] = false;
        }
      }

      if (setCenter) {
        app.interface.center = app.interface.left.images[0];
        setCenter = false;
      }
    }
  }
}