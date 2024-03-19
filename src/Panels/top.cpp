//
// Created by nathanial on 3/10/24.
//
#include "top.h"
#include "../Input/actions.h"
#include "../Utils/utils.h"

namespace Top {

  typedef int (*Menu_Button)(App::App &app);

  int New(App::App &app) {
    Action::New_Project(app);
    return 0;
  }
  int Open(App::App &app) {
    Action::Open_Project(app);
    return 1;
  }
  int Save(App::App &app) {
    Action::Save(app);
    return 4;
  }
  int Add_Image(App::App &app) {
    Action::Add_Image(app);
    return 4;
  }
  int Delete_Image(App::App &app) {
    Action::Remove_Image(app);
    return 4;
  }
  int Add_Sprites(App::App &app) {
    Action::Add_Sprites(app);
    return 3;
  }
  int Publish(App::App &app) {
    Action::Publish(app);
    return 5;
  }
  int Publish_As(App::App &app) {
    Action::Publish(app);
    return 5;
  }
  int Unused(App::App &app) {
    return 5;
  }

  std::array<Menu_Button, Graphics::menuSize> menuButtons = {
      New,
      Open,
      Save,
      Unused,
      Add_Image,
      Delete_Image,
      Add_Sprites,
      Unused,
      Publish,
      Publish_As,
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
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    SDL_RenderFillRectF(app.context.renderer, &app.panel.top.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
    SDL_RenderDrawRectF(app.context.renderer, &app.panel.top.panel);
    SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);

    for (const auto &btn: app.panel.top.buttons) {
      SDL_SetRenderDrawColor(app.context.renderer, 200, 200, 200, 255);
      SDL_RenderFillRectF(app.context.renderer, &btn.button);
      SDL_RenderCopyF(app.context.renderer, btn.texture, nullptr, &btn.button);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 255, 255, 255);
      SDL_RenderDrawRectF(app.context.renderer, &btn.button);
      SDL_SetRenderDrawColor(app.context.renderer, 0, 0, 0, 255);
    }
//    SDL_RenderCopyF(app.context.renderer, app.texture, nullptr, &app.panel.mainPanel.center.buttonBar);
  };
}