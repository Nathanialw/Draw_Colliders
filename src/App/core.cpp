//
// Created by nathanial on 3/9/24.
//

#include "core.h"
#include "../Input/mouse.h"

namespace App {

  void Init (App &app) {
    app.context = Graphics::CreateWindowAndRenderer();
    app.panel = Graphics::Set_Panels(app.context.window);

    Mouse::Set_Cursor(app, SDL_SYSTEM_CURSOR_ARROW);
  }

  void Close(App &app) {
    SDL_DestroyTexture(app.appData.center.texture.texture);
    for (auto &texture: app.appData.left.images) {
      SDL_DestroyTexture(texture.texture);
    }
    SDL_FreeCursor(app.cursor);
    SDL_DestroyRenderer(app.context.renderer);
    SDL_DestroyWindow(app.context.window);
    SDL_Quit();
  }
}