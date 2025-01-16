//
// Created by nathanial on 3/10/24.
//
#include "top.h"
#include "../Input/actions.h"
#include "../Utils/utils.h"

namespace Top {

  std::array<Action::Button , Graphics::menuSize> menuButtons = {
      Action::New_Project,
      Action::Open_Project,
      Action::Save,
      Action::Save_As,
      Action::UNIMPLEMENTED,
      Action::Add_Image,
      Action::Remove_Image,
      Action::Add_Images,
      Action::UNIMPLEMENTED,
      Action::Publish,
      Action::Publish_As,
  };

  bool Click_Menu_Button(App::App &app, const SDL_FPoint &point) {
    for (int i = 0; i < app.panel.top.buttons.size(); ++i) {
      if (Point_FRect_Intersect(point, app.panel.top.buttons[i].button)) {
        menuButtons[i](app);
        return true;
      }
    }
    return false;
  };

  void Render_Button_Bar(App::App &app) {
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.top.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.top.panel);
    Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);

    for (const auto &btn: app.panel.top.buttons) {
      if (btn.texture) {
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BACKGROUND]);
        SDL_RenderFillRectF(app.context.renderer, &btn.button);
        SDL_RenderCopyF(app.context.renderer, btn.texture, nullptr, &btn.button);
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BORDERS]);
        SDL_RenderDrawRectF(app.context.renderer, &btn.button);
        Graphics::Set_Render_Draw_Color(app.context.renderer, Graphics::color[Graphics::BLACK]);
      }
    }
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.buttonBar);
  };
}