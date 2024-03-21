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

void Log(const unsigned long &string) {
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

float distance(const SDL_FPoint &a, const SDL_FPoint &b) {
  return std::sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

float dotProduct(const SDL_FPoint &a, const SDL_FPoint &b) {
  return a.x * b.x + a.y * b.y;
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

float distanceToLineSegment(const SDL_FPoint &p, const SDL_FPoint &a, const SDL_FPoint &b) {
  SDL_FPoint ap = {p.x - a.x, p.y - a.y};
  SDL_FPoint ab = {b.x - a.x, b.y - a.y};

  float dot = dotProduct(ap, ab);
  float segLenSq = dotProduct(ab, ab);

  float t = dot / segLenSq;

  if (t < 0.0) {
    return distance(p, a);
  }

  if (t > 1.0) {
    return distance(p, b);
  }

  SDL_FPoint closestPoint = {a.x + t * ab.x, a.y + t * ab.y};
  return distance(p, closestPoint);
}

bool Point_FRect_Intersect(const SDL_FPoint &p, const SDL_FRect &r) {
  return ( (p.x >= r.x) && (p.x < (r.x + r.w)) && (p.y >= r.y) && (p.y < (r.y + r.h)) ) ? true : false;
}

SDL_FRect Rect_To_FRect(const SDL_Rect &rect) {
  SDL_FRect fRect;
  fRect.x = (float)rect.x;
  fRect.y = (float)rect.y;
  fRect.w = (float)rect.w;
  fRect.h = (float)rect.h;

  return fRect;
}

bool Circle_Intersect(float cx, float cy, float radius, const SDL_FRect &rect) {
  float closestX = (cx < rect.x ? rect.x : (cx > rect.x + rect.w ? rect.x + rect.w : cx));
  float closestY = (cy < rect.y ? rect.y : (cy > rect.y + rect.h ? rect.y + rect.h : cy));
  float dx = closestX - cx;
  float dy = closestY - cy;

  return ( dx * dx + dy * dy ) <= radius * radius;
}

/// this one only works for convex polygons, need one for concave polygons
bool PolygonOverlap_SAT(std::vector<SDL_FPoint> &r1, std::vector<SDL_FPoint> &r2) {
  std::vector<SDL_FPoint> *poly1 = &r1;
  std::vector<SDL_FPoint> *poly2 = &r2;

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

///works for concave polygons too
bool Point_In_Polygon(const std::vector<SDL_FPoint> &point, const std::vector<SDL_FPoint> &polygon) {
  int n = polygon.size();
  int counter = 0;
  double x_intercept = 0.0;
  float x = point[0].x;
  float y = point[0].y;

  auto p1 = polygon[0];
  for (int i = 0; i < n + 1; ++i) {
    auto p2 = polygon[i % n];
    if (y > std::min(p1.y, p2.y)) {
      if (y <= std::max(p1.y, p2.y)) {
        if (x <= std::max(p1.x, p2.x)) {
          if (p1.y != p2.y)
            x_intercept = (y - p1.y) * (p2.x - p1.x) / static_cast<double>(p2.y - p1.y) + p1.x;
          if (p1.x == p2.x || x <= x_intercept)
            counter++;
        }
      }
    }
    p1 = p2;
  }
  return counter % 2 == 1;
}
