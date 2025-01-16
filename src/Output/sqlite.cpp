//
// Created by nathanial on 3/10/24.
//


#include "iostream"

#include "sqlite/sqlite3.h"

#include "sqlite.h"

namespace SQLite {

  std::string Append_Quotes(const std::string &string) {
    return "'" + string + "'";
  }

  std::string Append_Quotes(const int &num) {
    return "'" + std::to_string(num) + "'";
  }

  std::string Append_Quotes(const float &num) {
    return "'" + std::to_string(num) + "'";
  }

  std::string Convert_Char(std::string column, const unsigned char *e) {
    const char *f = (const char *) e;
    if (f == NULL) {
      //      Utilities::Log("db::Convert_Char() \"" + column + "\" from DB NULL value, passthrough error");
      return "";
    }
    return std::string(reinterpret_cast<const char *>(e));
  }

  std::string Get_String(const unsigned char *text) {
    const char *s;
    s = (const char *) text;
    if (s != NULL) {
      return std::string(reinterpret_cast<const char *>(s));
    }
    return "";
  }

  bool Get_bool(std::string column, int value) {
    if (value == 0) {
      return false;
    }
    return true;
  }

  sqlite3* Init() {
    sqlite3* db;
    const char *db_filepath = "sqlite.db";
    int error = sqlite3_open(db_filepath, &db);
    if (error) {
      //if error then display error and close connection
      std::cout << "DB Open Error: " << sqlite3_errmsg(db) << std::endl;
      sqlite3_close(db);
    } else {
      std::cout << "Opened Database Successfully!" << std::endl;
      sqlite3_exec(db, "PRAGMA foreign_keys = ON;", nullptr, nullptr, nullptr);
      sqlite3_exec(db, "DROP TABLE IF EXISTS BODYDEF;", nullptr, nullptr, nullptr);
      sqlite3_exec(db, "DROP TABLE IF EXISTS FIXTURE;", nullptr, nullptr, nullptr);
      sqlite3_exec(db, "DROP TABLE IF EXISTS POLYGON;", nullptr, nullptr, nullptr);

      // Enable foreign key support
      return db;
    }
    return nullptr;
  }

  sqlite3* Create_Tables() {
    auto db = Init();

    std::string sql; // This line
    char* err;

    sql = "CREATE TABLE BODYDEF("\
          "ID               INTEGER     PRIMARY KEY         AUTOINCREMENT                 NOT NULL,"\
          "name             TEXT        UNIQUE                                            NOT NULL,"\
          "isDynamic        BOOL                                                          ,"\
          "isBullet         BOOL                                                          ,"\
          "numFixtures      INT                                                           );";

    auto rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
    if (err != SQLITE_OK)
      std::cout << err << std::endl;

    sql = "CREATE TABLE FIXTURE("\
          "ID                     INTEGER     PRIMARY KEY       AUTOINCREMENT                   NOT NULL,"\
          "bodyName               TEXT                                                          NOT NULL,"\
          "fixtureShape           TEXT                                                          ,"\
          "fixtureIndex           INT                                                           ,"\
          "numVertices            INT                                                           ,"\
          "isSensor               BOOL                                                          ,"\
          "density                INT                                                           ,"\
          "restitution            INT                                                           ,"\
          "friction               INT                                                           ,"\
          "bounce                 INT                                                           ,"\
          "filter_categoryBits    INT                                                           ,"\
          "filter_groupIndex      INT                                                           ,"\
          "filter_masksBits       INT                                                           ,"\
          "FOREIGN KEY (bodyName) REFERENCES BODYDEF (name)"
          ");";

    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
    if (err != SQLITE_OK)
      std::cout << err << std::endl;

    sql = "CREATE TABLE POLYGON("\
          "ID               INTEGER     PRIMARY KEY        AUTOINCREMENT                  NOT NULL,"\
          "bodyName         TEXT                                                          NOT NULL,"\
          "fixtureShape     TEXT                                                          ,"\
          "fixtureIndex     INT                                                           ,"\
          "vertexIndex      INT                                                           ,"\
          "x                INT                                                           ,"\
          "y                INT                                                           ,"\
          "FOREIGN KEY (bodyName) REFERENCES BODYDEF (name)"
          ");";

    rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
    if (err != SQLITE_OK)
      std::cout << err << std::endl;

    return db;
  }

  void Publish(App::App &app) {
    auto db = Create_Tables();

    std::array<std::string, Shape::SIZE> shapeStr = {"POINT", "CIRCLE", "LINE", "AABB", "POLYGON"};

    auto numBodies = app.interface.left.images.size();

    for (int i = 0; i < app.interface.left.images.size(); ++i) {
      const auto &body = app.interface.left.images[i];
      auto name = app.interface.left.imageNameStr[i];
      int shapeNum = 0;
      int numFixtures = body.shapes[Shape::AABB].size();
      numFixtures += body.shapes[Shape::POLYGON].size();
      numFixtures += body.shapes[Shape::POINT].size();
      numFixtures += body.shapes[Shape::CIRCLE].size();
      numFixtures += body.shapes[Shape::LINE].size();

      char* err;
//      std::string sql = "INSERT INTO BODYDEF (name, isDynamic, isBullet, numFixtures) VALUES (" + Append_Quotes(name) + ", " + Append_Quotes(body.isDynamic) + ", " + Append_Quotes(body.isBullet) + ", " + Append_Quotes(numFixtures) + ")";
//
//      auto rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
      if (err != SQLITE_OK)
        std::cout << err << std::endl;

      for (int j = 0; j < Graphics::SIZE; ++j) {
        for (int l = 0; l < body.shapes[j].size(); ++l) {
          const auto &fixture = body.shapes[j][l];
          std::string sql = "INSERT INTO FIXTURE (bodyName, fixtureIndex, fixtureShape, isSensor, numVertices) VALUES (" + Append_Quotes(name) + ", " + Append_Quotes(l) + ", " + Append_Quotes(shapeStr[j]) + ", " + Append_Quotes(fixture.isSensor) + ", " + Append_Quotes((int)fixture.vertices.size()) + ")";
            auto rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
          if (err != SQLITE_OK)
            std::cout << err << std::endl;
//          fixture->SetAttribute("density", shape.density);
//          fixture->SetAttribute("restitution", shape.restitution);
//          fixture->SetAttribute("friction", shape.friction);
//          fixture->SetAttribute("bounce", shape.bounce);
//
//          fixture->SetAttribute("filter_categoryBits", 1);
//          fixture->SetAttribute("filter_groupIndex", shape.group);
//          fixture->SetAttribute("filter_masksBits", 65535);


          for (int k = 0; k < fixture.vertices.size(); ++k) {
            const auto &vertex = fixture.vertices[k];
            std::string sql = "INSERT INTO POLYGON (bodyName, fixtureShape, fixtureIndex, vertexIndex, x, y) VALUES (" + Append_Quotes(name) + ", " + Append_Quotes(shapeStr[j]) + ", " + Append_Quotes(l) + ", " + Append_Quotes(k) + ", " + Append_Quotes(vertex.x) + ", " + Append_Quotes(vertex.y) + ")";
            auto rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err);
            if (err != SQLITE_OK)
              std::cout << err << std::endl;
          }
        }
      }
    }
    sqlite3_close(db);
  }
}