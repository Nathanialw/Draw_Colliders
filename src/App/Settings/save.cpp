//
// Created by nathanial on 3/13/24.
//

#include "serialise.h"
#include "save.h"
#include "../../Graphics/text.h"
#include "../../Graphics/graphics.h"
#include "../../../lib/nativefiledialog/nfd.h"
#include <thread>

namespace Save {

  bool Save(App::App &app, const std::string &fileName) {
    std::array<std::string, 5> shapeStr = {"point", "circle", "lineSegment", "aabb", "polygon"};
    //saves the working center image
    if (app.interface.left.images.empty())
      return false;

    app.interface.left.images[app.interface.center.index] = app.interface.center;

    //resets the in memory data to save the file
    Serialise::Datafile datafile;
    app.datafile = datafile;

    app.datafile["header"]["name"].Set_String(fileName);
    //maybe more than 1
    app.datafile["header"]["folder"].Set_String("path to folder with images");
    app.datafile["header"]["image_count"].Set_Int(app.interface.left.imagePathStr.size());

    for (int i = 0; i < app.interface.left.imagePathStr.size(); ++i) {
      auto &image = app.datafile["image[" + std::to_string(i) + "]"];
      image["name"].Set_String(app.interface.left.imageNameStr[i]);
      image["path"].Set_String(app.interface.left.imagePathStr[i]);
      image["scale"].Set_Real(app.interface.left.images[i].texture.scale);
      int k = 0;
      image[shapeStr[0]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::POINT].size());
      for (const auto &point: app.interface.left.images[i].shapes[Graphics::POINT]) {
        image[shapeStr[0]][std::to_string(k)]["x"].Set_Real(point.vertices[0].x);
        image[shapeStr[0]][std::to_string(k)]["y"].Set_Real(point.vertices[0].y);
        k++;
      }
      k = 0;
      image[shapeStr[1]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::CIRCLE].size());
      for (const auto &circle: app.interface.left.images[i].shapes[Graphics::CIRCLE]) {
        image[shapeStr[1]][std::to_string(k)]["x"].Set_Real(circle.vertices[0].x);
        image[shapeStr[1]][std::to_string(k)]["y"].Set_Real(circle.vertices[0].y);
        image[shapeStr[1]][std::to_string(k)]["r"].Set_Real(circle.vertices[1].y - circle.vertices[0].y);
        k++;
      }
      k = 0;
      image[shapeStr[2]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::LINE].size());
      for (const auto &line: app.interface.left.images[i].shapes[Graphics::LINE]) {
        int vertexCount = 0;
        for (const auto &vertex : line.vertices) {
          image[shapeStr[2]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          image[shapeStr[2]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
      k = 0;
      image[shapeStr[3]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::AABB].size());
      for (const auto &aabb: app.interface.left.images[i].shapes[Graphics::AABB]) {
        int vertexCount = 0;
        for (const auto &vertex : aabb.vertices) {
          image[shapeStr[3]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          image[shapeStr[3]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
      k = 0;
      image[shapeStr[4]]["shape_count"].Set_Int(app.interface.left.images[i].shapes[Graphics::POLYGON].size());
      for (const auto &polygon: app.interface.left.images[i].shapes[Graphics::POLYGON]) {
        int vertexCount = 0;
        image[shapeStr[4]][std::to_string(k)]["vertex_count"].Set_Int(polygon.vertices.size());
        for (const auto &vertex : polygon.vertices) {
          image[shapeStr[4]][std::to_string(k)][std::to_string(vertexCount)]["x"].Set_Real(vertex.x);
          image[shapeStr[4]][std::to_string(k)][std::to_string(vertexCount)]["y"].Set_Real(vertex.y);
          vertexCount++;
        }
        k++;
      }
    }
    app.datafile.Write(app.datafile, fileName);
    return true;
  }

  // Save As
  void Save_File_Async(App::App &app, bool &loaded) {
    nfdchar_t *outPath;
    auto result = NFD_SaveDialog("txt", nullptr, &outPath);

    loaded = true;
    if (result == NFD_CANCEL ||  result == NFD_ERROR ) {
      free(outPath);
    }
    else {
      app.saveName = outPath;
      if (!app.saveName.contains(".txt"))
        app.saveName += ".txt";
      Save(app, app.saveName);
      free(outPath);
    }
  }

//  Load As
  bool Save_As(App::App &app) {

    bool loaded = false;
    auto thread_func = [&app, &loaded]() {
      Save_File_Async(app, loaded);
    };

    Show_Overlay(app.context);
    std::thread LOAD_THREAD(thread_func);

    SDL_Event event;
    Graphics::Wait(loaded);
    LOAD_THREAD.join();
    return true;
  }

  void Load (App::App &app, const std::string &fileName) {
    //maybe have a dialog box here to say why the load failed
    if (!app.datafile.Read(app.datafile, fileName))
      return;

    std::array<std::string, 5> shapeStr = {"point", "circle", "lineSegment", "aabb", "polygon"};
    //maybe have a dialog box here to say why the load failed
    if (app.datafile["header"]["image_count"].Get_String().empty())
      return;
    //minus the header
    auto num = app.datafile["header"]["image_count"].Get_Int();
    //clear values
    Data::Left newLeft;
    app.interface.left = newLeft;
    app.texture.shapes.resize(num);

    for (int p = 0; p < num; ++p) {
      if (app.interface.left.imageNameStr.size() <= p)
        app.interface.left.imageNameStr.resize(p + 1);
      if (app.interface.left.imagePathStr.size() <= p)
        app.interface.left.imagePathStr.resize(p + 1);
      if (app.interface.left.images.size() <= p)
        app.interface.left.images.resize(p + 1);

      auto &image = app.datafile["image[" + std::to_string(p) + "]"];
      std::string name = image["name"].Get_String();
      std::string path = image["path"].Get_String();

      //load
      app.interface.left.images[p].index = p;
      app.interface.left.images[p].texture.scale = app.datafile["image[" + std::to_string(p) + "]"]["scale"].Get_Real();
      app.interface.left.imageNameStr[p] = name;
      app.interface.left.imagePathStr[p] = path;
//        load the image to the center if there is no image

      auto &shapes = app.interface.left.images[p];
      //need to save the number of each shape
      int shape_count = image[shapeStr[0]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::POINT].size() <= shape_count)
        shapes.shapes[Graphics::POINT].resize(shape_count);
      for (int l = 0; l < shape_count; ++l) {
        shapes.shapes[Graphics::POINT][l] = Point::Create((float)image[shapeStr[0]][std::to_string(l)]["x"].Get_Real(), (float)image[shapeStr[0]][std::to_string(l)]["y"].Get_Real());
      }

      shape_count = image[shapeStr[1]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::CIRCLE].size() <= shape_count) {
        shapes.shapes[Graphics::CIRCLE].resize(shape_count);
        app.texture.shapes[p][Graphics::CIRCLE].resize(shape_count);
      }
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
      if (shape_count != 0 && shapes.shapes[Graphics::AABB].size() <= shape_count) {
        shapes.shapes[Graphics::AABB].resize(shape_count);
        app.texture.shapes[p][Graphics::AABB].resize(shape_count);
      }
      for (int l = 0; l < shape_count; ++l) {
        shapes.shapes[Graphics::AABB][l].vertices.resize(4);
        shapes.shapes[Graphics::AABB][l].moving.resize(4);
        for (int j = 0; j < 4; ++j) {
          shapes.shapes[Graphics::AABB][l].vertices[j] = {(float)image[shapeStr[3]][std::to_string(l)][std::to_string(j)]["x"].Get_Real(), (float)image[shapeStr[3]][std::to_string(l)][std::to_string(j)]["y"].Get_Real()};
          shapes.shapes[Graphics::AABB][l].moving[j] = false;
        }
      }

      shape_count = image[shapeStr[4]]["shape_count"].Get_Int();
      if (shape_count != 0 && shapes.shapes[Graphics::POLYGON].size() <= shape_count) {
        shapes.shapes[Graphics::POLYGON].resize(shape_count);
        app.texture.shapes[p][Graphics::POLYGON].resize(shape_count);
      }
      for (int l = 0; l < shape_count; ++l) {
        int vertex_count = image[shapeStr[4]][std::to_string(l)]["vertex_count"].Get_Int();
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


    }
  }

   void Open_File_Async(App::App &app, bool &loaded) {
    nfdchar_t *outPath;
    nfdresult_t result;
    result = NFD_OpenDialog("txt", nullptr, &outPath);

    loaded = true;
    if (result == NFD_CANCEL ||  result == NFD_ERROR ) {
      free(outPath);
    }
    else {
      app.saveName = outPath;
      Load(app, app.saveName);
      free(outPath);
    }
  }

  bool Load_Textures(App::App &app) {
    bool setCenter = true;
    auto& images = app.interface.left;
    for (int k = 0; k < images.images.size(); ++k) {
      images.images[k].texture.texture = Graphics::Load_Texture(app.context, images.imagePathStr[k]);

      if (setCenter) {
        app.interface.center = app.interface.left.images[k];
        Data::Shape_List shapeList;
        for (int i = 0; i < Graphics::Shape::SIZE; ++i) {
          for (int j = 0; j < app.interface.center.shapes[i].size(); ++j) {
            shapeList.shapeList[i].push_back(std::to_string(j));
          }
        }
        app.interface.shapeList = shapeList;
        setCenter = false;
      }
    }
    return true;
  }

//  Load As
  bool Load_As(App::App &app) {

    bool loaded = false;

    //cannot load textures in another thread
    auto thread_func = [&app, &loaded]() {
      Open_File_Async(app, loaded);
    };

    Show_Overlay(app.context);
    std::thread LOAD_THREAD(thread_func);
    Graphics::Wait(loaded);
    LOAD_THREAD.join();

    Load_Textures(app);
    return true;
  }

}