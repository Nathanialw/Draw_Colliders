
//

// Created by nathanial on 3/10/24.
//

#include "../../lib/tinyxml2/tinyxml2.h"
#include "string"
#include "xml.h"

using namespace tinyxml2;

namespace XML {

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif



  void Publish(App::App &app) {
    std::array<std::string, Graphics::SIZE> shapeStr = {"POINT", "CIRCLE", "LINE", "AABB", "POLYGON"};

    XMLDocument xmlDoc;

    XMLNode * pRoot = xmlDoc.NewElement("bodydef");
    xmlDoc.InsertFirstChild(pRoot);

    XMLElement* bodies = xmlDoc.NewElement("bodies");
    bodies->SetAttribute("numBodies", app.interface.left.images.size());

    for (int i = 0; i < app.interface.left.images.size(); ++i) {
      auto name = app.interface.left.imageNameStr[i];
      int shapeNum = 0;

      XMLElement* body = bodies->InsertNewChildElement("body");

      body->SetAttribute("name", name.c_str());
      body->SetAttribute("dynamic", false);

      int numFixtures = app.interface.left.images[i].shapes[Graphics::AABB].size();
      numFixtures += app.interface.left.images[i].shapes[Graphics::POLYGON].size();
      numFixtures += app.interface.left.images[i].shapes[Graphics::POINT].size();
      numFixtures += app.interface.left.images[i].shapes[Graphics::CIRCLE].size();
      numFixtures += app.interface.left.images[i].shapes[Graphics::LINE].size();
      body->SetAttribute("numFixtures", numFixtures);
      body->SetAttribute("isDynamic", app.interface.left.images[i].isDynamic);
      body->SetAttribute("isBullet", app.interface.left.images[i].isBullet);

//      for (const auto &shapes :app.interface.left.images[i].shapes) {
      for (int j = 0; j < Graphics::SIZE; ++j) {
        for (const auto &shape : app.interface.left.images[i].shapes[j]) {
          XMLElement* fixture = body->InsertNewChildElement("fixture");

          fixture->SetAttribute("isSensor", shape.isSensor);
          fixture->SetAttribute("density", shape.density);
          fixture->SetAttribute("restitution", shape.restitution);
          fixture->SetAttribute("friction", shape.friction);
          fixture->SetAttribute("bounce", shape.bounce);
          fixture->SetAttribute("shape", shapeStr[j].c_str());

          fixture->SetAttribute("filter_categoryBits", 1);
          fixture->SetAttribute("filter_groupIndex", shape.group);
          fixture->SetAttribute("filter_masksBits", 65535);

//          fixture->SetAttribute("numPolygons", shape);

          XMLElement* polygon = fixture->InsertNewChildElement("polygon");

          polygon->SetAttribute("numVertices", shape.vertices.size());


          for (const auto &vertex : shape.vertices) {
            XMLElement* pVertex = polygon->InsertNewChildElement("vertex");
            pVertex->SetAttribute("x", vertex.x);
            pVertex->SetAttribute("y", vertex.y);
          }
          shapeNum++;
        }
      }
    }

    pRoot->InsertEndChild(bodies);

    xmlDoc.SaveFile("SavedData.xml");
  }
}