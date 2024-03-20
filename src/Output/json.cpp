//
// Created by nathanial on 3/10/24.
//
#include "fstream"
#include "iostream"

#include "../../lib/nlohmann/json.hpp"

#include "../App/core.h"

using json = nlohmann::json;

namespace JSON {

  void Publish(App::App &app) {

    json j;

    for (int i = 0; i < app.interface.left.images.size(); ++i) {

      auto name = app.interface.left.imageNameStr[i];
      auto density = app.interface.left.density;
      auto bounce = app.interface.left.bounce;
      auto isSensor = app.interface.left.isSensor;
      auto friction = app.interface.left.friction;

      int shapeNum = 0;
      for (const auto &shapes :app.interface.left.images[i].shapes) {
          for (const auto &shape : shapes) {

          j[name][shapeNum]["density"] = density;
          j[name][shapeNum]["friction"] = friction;
          j[name][shapeNum]["bounce"] = bounce;
          j[name][shapeNum]["isSensor"] = isSensor;
          j[name][shapeNum]["filter"] = {{"categoryBits", 1},
                               {"maskBits",     65535}};

          for (int l = 0; l < shape.vertices.size(); ++l) {
            j[name][shapeNum]["shape"].push_back({{"x", shape.vertices[l].x}, {"y", shape.vertices[l].y}});
          }
          shapeNum++;
        }
      }
    }

    // count elements
    auto s = j.size();
    j["size"] = s;

    const std::string fileName = "json.json";
    // pretty print with indent of 4 spaces
    std::ofstream file(fileName);
    file << std::setw(4) << j;
    std::cout << std::setw(4) << j;


  }

}