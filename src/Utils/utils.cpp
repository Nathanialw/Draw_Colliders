//
// Created by nathanial on 3/9/24.
//
#include "utils.h"
#include "iostream"
#include "math.h"
#include "utils.h"

void Log(const auto &string) {
  std::cout << string << std::endl;
}

void Log(const int &string) {
  std::cout << string << std::endl;
}

void Log(const float &string) {
  std::cout << string << std::endl;
}

void Log(const std::string &string) {
  std::cout << string << std::endl;
}

void Log(const char* &string) {
  std::cout << string << std::endl;
}

bool Rect_Intersect(const SDL_FRect &entity, const SDL_FRect &target) {
  if ((entity.y <= target.y + target.h) &&
      (entity.x <= target.x + target.w) &&
      (entity.y + entity.h >= target.y) &&
      (entity.x + entity.w >= target.x)) {
    return true;
  }
  return false;
};

bool PolygonOverlap_SAT(std::vector<f2> &r1, std::vector<f2> &r2) {
  std::vector<f2> *poly1 = &r1;
  std::vector<f2> *poly2 = &r2;

  for (int shape = 0; shape < 2; shape++) {
    if (shape == 1) {
      poly1 = &r2;
      poly2 = &r1;
    }

    for (int a = 0; a < poly1->size(); a++) {
      int b = (a + 1) % poly1->size();
      f2 axisProj = {-(poly1->at(b).y - poly1->at(a).y), poly1->at(b).x - poly1->at(a).x};
      float d = sqrtf(axisProj.x * axisProj.x + axisProj.y * axisProj.y);
      axisProj = {axisProj.x / d, axisProj.y / d};

      // Work out min and max 1D points for r1
      float min_r1 = INFINITY, max_r1 = -INFINITY;
      for (int p = 0; p < poly1->size(); p++) {
        float q = (poly1->at(p).x * axisProj.x + poly1->at(p).y * axisProj.y);
        min_r1 = std::min(min_r1, q);
        max_r1 = std::max(max_r1, q);
      }

      // Work out min and max 1D points for r2
      float min_r2 = INFINITY, max_r2 = -INFINITY;
      for (int p = 0; p < poly2->size(); p++) {
        float q = (poly2->at(p).x * axisProj.x + poly2->at(p).y * axisProj.y);
        min_r2 = std::min(min_r2, q);
        max_r2 = std::max(max_r2, q);
      }

      if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
        return false;
    }
  }

  return true;
}
