//
// Created by nathanial on 3/10/24.
//

#include "../Graphics/graphics.h"
#include "../App/core.h"

namespace Bottom {
  bool Bottom_Panel() {
    return false;
  };

  bool Render(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::COOL_GRAY]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.bottom);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::LIGHT_GRAY]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.bottom);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
    return true;
  }
}