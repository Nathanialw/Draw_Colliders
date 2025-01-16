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

struct i2 {
  int x;
  int y;
};

void Log(const auto &string);
void Log(const int &string);
void Log(const float &string);
void Log(const std::string &string);
void Log(const char* &string);

auto Get_Max(const auto &a, const auto &b);
int Get_Max(const int &a, const int &b);
int Get_Min(const int &a, const int &b);

SDL_FRect Rect_To_FRect(const SDL_Rect &rect);


float distanceToLineSegment(const SDL_FPoint &p, const SDL_FPoint &a, const SDL_FPoint &b);
bool Rect_Intersect(const SDL_FRect &entity, const SDL_FRect &target);
bool Point_FRect_Intersect(const SDL_FPoint &p, const SDL_FRect &r);
bool Circle_Intersect(float cx, float cy, float radius, const SDL_FRect &rect);
bool PolygonOverlap_SAT(std::vector<SDL_FPoint> &r1, std::vector<SDL_FPoint> &r2);
bool Point_In_Polygon(const std::vector<SDL_FPoint> &point, const std::vector<SDL_FPoint> &polygon);
