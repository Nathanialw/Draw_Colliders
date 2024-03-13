//
// Created by nathanial on 3/11/24.
//
#include "text.h"

namespace Text {
  FC_Font* Load_Font(SDL_Renderer *renderer) {
    FC_Font *font = FC_CreateFont();
    FC_LoadFont(font, renderer, "assets/fonts/EagleLake-Regular.ttf", 15, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL);
    return font;
  }

  void Render(SDL_Renderer *renderer, FC_Font* font, const char* text, const float &x, const float &y) {
    FC_Draw(font, renderer, x, y, text);
  }

  void Render(SDL_Renderer *renderer, FC_Font* font, const char* text, const SDL_Rect &rect) {
    FC_DrawBoxContainedLine(font, renderer, rect, text);
  }

  std::string Get_File_Name(const std::string &filePath) {
    std::string string = filePath;

    for (int i = string.size() - 1; i > 0; --i) {
      if (string[i] == '.')
        string.erase(i, string.size());
      if (string[i] == '/' || string[i] == '\\') {
        string.erase(0, i + 1);
        return string;
      }
    }

    return "";
  }
}