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

    // count elements
    auto numBodies = j.size();
    j["size"] = numBodies;

    for (int i = 0; i < app.interface.left.images.size(); ++i) {
      auto name = app.interface.left.imageNameStr[i];
      int shapeNum = 0;
      for (const auto &shapes :app.interface.left.images[i].shapes) {
          for (const auto &shape : shapes) {

          j[name][shapeNum]["density"] = shape.density;
          j[name][shapeNum]["restitution"] = shape.bounce;
          j[name][shapeNum]["group"] = shape.friction;
          j[name][shapeNum]["isDynamic"] = shape.isSensor;
          j[name][shapeNum]["isBullet"] = shape.isBullet;
          j[name][shapeNum]["friction"] = shape.restitution;
          j[name][shapeNum]["bounce"] = shape.group;
          j[name][shapeNum]["isSensor"] = shape.isDynamic;
          j[name][shapeNum]["filter"] = {{"categoryBits", 1},
                               {"maskBits",     65535}};

          for (const auto &vertex : shape.vertices) {
            j[name][shapeNum]["shape"].push_back({{"x", vertex.x}, {"y", vertex.y}});
          }
          shapeNum++;
        }
      }
    }

    //open dialogue
    const std::string fileName = "json.json";

    std::ofstream file(fileName);
    file << std::setw(4) << j;
//    std::cout << std::setw(4) << j;
  }
}