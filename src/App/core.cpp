//
// Created by nathanial on 3/9/24.
//

#include "core.h"
#include "../Input/mouse.h"
#include "../Graphics/text.h"

namespace App {

  void Set_Textures(App &app) {
    app.panel.mainPanel.center.buttonBar.buttons[0].texture = app.texture.location;
    app.panel.mainPanel.center.buttonBar.buttons[1].texture = app.texture.point;
    app.panel.mainPanel.center.buttonBar.buttons[2].texture = app.texture.nodes;
    app.panel.mainPanel.center.buttonBar.buttons[3].texture = app.texture.vector;
    app.panel.mainPanel.center.buttonBar.buttons[4].texture = app.texture.pentagon;
//    app.panel.mainPanel.center.buttonBar.buttons[5].texture = app.texture;
    app.panel.mainPanel.center.buttonBar.buttons[6].texture = app.texture.deleteShape;
    app.panel.mainPanel.center.buttonBar.buttons[7].texture = app.texture.addVertex;
    app.panel.mainPanel.center.buttonBar.buttons[8].texture = app.texture.deleteVertex;

    app.panel.top.buttons[0].texture = app.texture.newDocument;
    app.panel.top.buttons[1].texture = app.texture.open;
    app.panel.top.buttons[2].texture = app.texture.save;
    app.panel.top.buttons[3].texture = app.texture.saveAs;
//    app.panel.top.buttons[4].texture = app.texture.addFolder;
    app.panel.top.buttons[5].texture = app.texture.addImage;
    app.panel.top.buttons[6].texture = app.texture.deleteImage;
    app.panel.top.buttons[7].texture = app.texture.addFolder;
//    app.panel.top.buttons[8].texture = app.texture.publish;
    app.panel.top.buttons[9].texture = app.texture.publish;
    app.panel.top.buttons[10].texture = app.texture.publishAs;
  }

  void New(App &app) {
    Data::Left newLeft;
    Data::Center newCenter;
    Data::Right newRight;

    app.interface.left = newLeft;
    app.interface.center = newCenter;
    app.interface.right = newRight;
    //reset shape list
    Data::Shape_List shapeList;
    app.interface.shapeList = shapeList;
    Vertex vertex;
    app.vertex = vertex;
    app.selectedVertex = vertex;
    Shape shape;
    app.selectedShape = shape;
    app.imageIndex = 0;
    app.filterImages = false;
    app.interface.left.filteredIndexes.clear();
    app.moveImage = false;
    app.moveVertex = false;
    app.initialPosition = {0, 0};
    app.offset = {0.0f, 0.0f};
    app.zoomToMouse = true;
    app.menuOpen = false;
    app.saveName.clear();
    app.interface.right.optionName = {
        "Export:",
        "format",
        "",
        "Body:",
        "body_label",
        "isDynamic",
        "",
        "Fixture:",
        "isSensor",
        "isBullet",
        "",
        "filter_category_bits",
        "filter_mask_bits",
        "bounce",
        "density",
        "group",
        "restitution",
        "friction",
    };

    app.interface.right.optionInput = {
        Data::Option_Type::LABEL,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NONE,
        Data::Option_Type::LABEL,
        Data::Option_Type::TEXTINPUT,
        Data::Option_Type::CHECKBOX,
        Data::Option_Type::NONE,
        Data::Option_Type::LABEL,
        Data::Option_Type::CHECKBOX,
        Data::Option_Type::CHECKBOX,
        Data::Option_Type::NONE,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NUMINPUT,
        Data::Option_Type::NUMINPUT,
    };
  }

  void Init (App &app) {
    app.context = Graphics::CreateWindowAndRenderer();
    app.panel = Graphics::Set_Panels(app.context.window, app.uiPanels);
    app.texture = Graphics::Load_Icons(app.context.renderer);
    New(app);
    Set_Textures(app);
    app.context.font = Text::Load_Font(app.context.renderer);
    Mouse::Set_Cursor(app, SDL_SYSTEM_CURSOR_ARROW);
  }

  void Close(App &app) {
    SDL_DestroyTexture(app.interface.center.texture.texture);
    for (auto &texture: app.interface.left.images) {
      SDL_DestroyTexture(texture.texture.texture);
    }
    SDL_FreeCursor(app.cursor);
    SDL_DestroyRenderer(app.context.renderer);
    SDL_DestroyWindow(app.context.window);
    SDL_Quit();
  }



  Offsets Calc_Offset(const App &app) {
    SDL_Point size;
    SDL_QueryTexture(app.interface.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
    float w = (float) size.x * app.interface.center.texture.scale;
    float h = (float) size.y * app.interface.center.texture.scale;

    return {
        (app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w)) / 2.0f) + (w / 2.0f),
        (app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h)) / 2.0f) + (h / 2.0f),
        (float) app.vertexRadius * app.interface.center.texture.scale
    };
  }

  SDL_FPoint Offset_From_Image_Center(const App &app, const SDL_FPoint &point) {
    SDL_Point size;
    SDL_QueryTexture(app.interface.center.texture.texture, nullptr, nullptr, &size.x, &size.y);
    float w = (float) size.x * app.interface.center.texture.scale;
    float h = (float) size.y * app.interface.center.texture.scale;

    auto x = (app.panel.mainPanel.center.image.x + ((app.panel.mainPanel.center.image.w - w)) / 2.0f) + (w / 2.0f);
    auto y = (app.panel.mainPanel.center.image.y + ((app.panel.mainPanel.center.image.h - h)) / 2.0f) + (h / 2.0f);
    auto gg = -(x - point.x) / app.interface.center.texture.scale;
    auto hh = -(y - point.y) / app.interface.center.texture.scale;
    gg += (float) app.interface.center.texture.offset.x / app.interface.center.texture.scale;
    hh += (float) app.interface.center.texture.offset.y / app.interface.center.texture.scale;
    return {gg, hh};
  }

  SDL_FRect Vertex_To_Rect(const App &app, const SDL_FPoint &vertex, const Offsets &o, bool moveVertex) {
    SDL_FRect rect;
    rect.x = (vertex.x * app.interface.center.texture.scale) + o.x - o.r;
    rect.y = (vertex.y * app.interface.center.texture.scale) + o.y - o.r;
    rect.w = o.r * 2.0f;
    rect.h = o.r * 2.0f;
    if (app.moveImage || moveVertex || app.zoomToMouse) {
      rect.x -= (float) app.offset.x;
      rect.y -= (float) app.offset.y;
    }
    rect.x -= (float) app.interface.center.texture.offset.x;
    rect.y -= (float) app.interface.center.texture.offset.y;
    return rect;
  }

  SDL_FPoint Vertex_To_Screen(const App &app, const SDL_FPoint &vertex, const Offsets &o, bool moveVertex) {
    SDL_FRect rect;
    rect.x = (vertex.x * app.interface.center.texture.scale) + o.x;
    rect.y = (vertex.y * app.interface.center.texture.scale) + o.y;
    if (app.moveImage || moveVertex || app.zoomToMouse) {
      rect.x -= (float) app.offset.x;
      rect.y -= (float) app.offset.y;
    }
    rect.x -= (float) app.interface.center.texture.offset.x;
    rect.y -= (float) app.interface.center.texture.offset.y;
    return {rect.x, rect.y};
  }

  Vertex Get_Vertex(App &app, const SDL_FRect &cursor) {
    auto o = Calc_Offset(app);
    auto &shape = app.interface.center.shapes;

    for (int k = 0; k < Graphics::SIZE; ++k) {
      if (k == Graphics::CIRCLE) {
        for (int i = 0; i < shape[k].size(); ++i) {
          auto vRect = Vertex_To_Rect(app, shape[k][i].vertices[1], o, shape[k][i].moving[1]);
          if (SDL_HasIntersectionF(&vRect, &cursor)) {
            shape[k][i].moving[1] = true;
            return {(Graphics::Shape) k, i, 1};
          }
        }
      }
      else {
        for (int i = 0; i < shape[k].size(); ++i) {
          for (int j = 0; j < shape[k][i].vertices.size(); ++j) {
            auto vRect = Vertex_To_Rect(app, shape[k][i].vertices[j], o, shape[k][i].moving[j]);
            if (SDL_HasIntersectionF(&vRect, &cursor)) {
              shape[k][i].moving[j] = true;
              return {(Graphics::Shape)k, i, j};
            }
          }
        }
      }
    }
    return {Graphics::SIZE, 0};
  }

  Vertex Move (Data::Center &shape, const int &k, const int &i) {
    for (auto &&move: shape.shapes[k][i].moving)
      move = true;
    return {(Graphics::Shape) k, i, -1};
  }

  Vertex Check_If_Selected(App &app, const SDL_FRect &cursor, const Graphics::Shape &selectedShape, const int &index) {
    if (selectedShape != Graphics::SIZE) {
      auto &shape = app.interface.center;
      auto o = Calc_Offset(app);
      std::vector<SDL_FPoint> mouseVertexes;
      mouseVertexes.push_back({cursor.x, cursor.y});
      mouseVertexes.push_back({cursor.x + cursor.w, cursor.y});
      mouseVertexes.push_back({cursor.x + cursor.w, cursor.y + cursor.h});
      mouseVertexes.push_back({cursor.x, cursor.y + cursor.h});
      std::vector<SDL_FPoint> shapeVertexes;

      for (int j = 0; j < shape.shapes[selectedShape][index].vertices.size(); ++j) {
        SDL_FPoint v = Vertex_To_Screen(app, shape.shapes[selectedShape][index].vertices[j], o, shape.shapes[selectedShape][index].moving[j]);
        shapeVertexes.emplace_back(v);
      }
      if (selectedShape == Graphics::CIRCLE) {
          if (Circle_Intersect(shapeVertexes[0].x, shapeVertexes[0].y, shapeVertexes[1].y - shapeVertexes[0].y, cursor))
            return Move(shape, selectedShape, index);
        }
      else if (selectedShape == Graphics::POLYGON) {
        if (Point_In_Polygon(mouseVertexes, shapeVertexes))
          return Move(shape, selectedShape, index);
      }
      else if (selectedShape == Graphics::AABB || selectedShape == Graphics::LINE) {
        if (PolygonOverlap_SAT(mouseVertexes, shapeVertexes))
          return Move(shape, selectedShape, index);
      }
    }
    return {Graphics::SIZE, 0};
  }

  Vertex Get_Shape(App &app, const SDL_FRect &cursor) {
    auto &shapes = app.interface.center.shapes;

    //check if I already have a shape first, return it if I still have it selected
    auto shape = Check_If_Selected(app, cursor, app.selectedShape.shape, app.selectedShape.indexPolygon);
    if (shape.shape != Graphics::SIZE)
      return shape;

    for (int k = 0; k < Graphics::SIZE; ++k) {
      for (int i = shapes[k].size() - 1; i >= 0; --i) {
        shape = Check_If_Selected(app, cursor, (Graphics::Shape)k, i);
        if (shape.shape != Graphics::SIZE)
          return shape;
      }
    }
    return {Graphics::SIZE, 0};
  }

}