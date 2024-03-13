#pragma once
//
// Created by nathanial on 3/11/24.
//

#ifndef BOX2D_COLLIDERS_TEXT_H
#define BOX2D_COLLIDERS_TEXT_H

#endif //BOX2D_COLLIDERS_TEXT_H
#include "../../lib/SDL_FontCache/SDL_FontCache.h"
#include "string"


namespace Text {
  FC_Font* Load_Font(SDL_Renderer *renderer);

  std::string Get_File_Name(const std::string &filePath);

  FC_Rect Render(SDL_Renderer *renderer, FC_Font* font, const char* text, const float &x, const float &y);
  FC_Rect Render(SDL_Renderer *renderer, FC_Font* font, const char* text, const SDL_Rect &rect);
}