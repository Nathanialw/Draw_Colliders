#pragma once
//
// Created by nathanial on 3/9/24.
//
#include "vector"
#include "SDL2/SDL.h"
#include "string"

struct f2 {
  float x;
  float y;
};

void Log(const auto &string);
void Log(const int &string);
void Log(const float &string);
void Log(const std::string &string);
void Log(const char* &string);

bool Rect_Intersect(const SDL_FRect &entity, const SDL_FRect &target);
bool PolygonOverlap_SAT(std::vector<f2> &r1, std::vector<f2> &r2);
