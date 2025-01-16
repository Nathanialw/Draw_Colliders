//
// Created by nathanial on 3/9/24.
//

#include "core.h"
#include "../Input/mouse.h"
#include "../Graphics/text.h"
#include "../Graphics/graphics.h"

#include "SDL2/SDL_image.h"

namespace App {


  Texture Load_Icons(SDL_Renderer *renderer) {
    Texture texture;

    texture.save = IMG_LoadTexture(renderer, "assets/icons/disk.png");
    texture.saveAs = IMG_LoadTexture(renderer, "assets/icons/save-as.png");
    texture.newDocument = IMG_LoadTexture(renderer, "assets/icons/new-document.png");
    texture.open = IMG_LoadTexture(renderer, "assets/icons/folder.png");

    texture.show = IMG_LoadTexture(renderer, "assets/icons/view.png");
    texture.hide = IMG_LoadTexture(renderer, "assets/icons/hide.png");

    texture.location = IMG_LoadTexture(renderer, "assets/icons/location.png");
    texture.point = IMG_LoadTexture(renderer, "assets/icons/point.png");
    texture.nodes = IMG_LoadTexture(renderer, "assets/icons/nodes.png");
    texture.vector = IMG_LoadTexture(renderer, "assets/icons/vector.png");
    texture.pentagon = IMG_LoadTexture(renderer, "assets/icons/pentagon.png");

    texture.deleteShape = IMG_LoadTexture(renderer, "assets/icons/minus.png");
    texture.deleteVertex = IMG_LoadTexture(renderer, "assets/icons/minus-location.png");
    texture.addVertex = IMG_LoadTexture(renderer, "assets/icons/add.png");

    texture.up = IMG_LoadTexture(renderer, "assets/icons/up-loading.png");
    texture.addFolder = IMG_LoadTexture(renderer, "assets/icons/archive.png");

    texture.vertex = IMG_LoadTexture(renderer, "assets/icons/vertex.png");
    texture.circle = IMG_LoadTexture(renderer, "assets/icons/circle.png");
    texture.addImage = IMG_LoadTexture(renderer, "assets/icons/add_image.png");
    texture.deleteImage = IMG_LoadTexture(renderer, "assets/icons/remove_image.png");
    texture.publish = IMG_LoadTexture(renderer, "assets/icons/publish.png");
    texture.publishAs = IMG_LoadTexture(renderer, "assets/icons/publish_as.png");

    texture.uncheckedBox = IMG_LoadTexture(renderer, "assets/icons/unchecked.png");
    texture.checkedBox = IMG_LoadTexture(renderer, "assets/icons/checkbox.png");

    texture.alphaTexture = IMG_LoadTexture(renderer, "assets/images/fogOfWar.png");
    SDL_SetTextureBlendMode(texture.alphaTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(texture.alphaTexture, 0);
    return texture;
  }

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
    Shape::Shape_List shapeList;
    app.interface.shapeList = shapeList;
    Shape::Vertex vertex;
    app.vertex = vertex;
    app.selectedVertex = vertex;
    Shape::Fixture shape;
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
    app.texture = Load_Icons(app.context.renderer);
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

}