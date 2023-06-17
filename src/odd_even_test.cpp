#include <vector>
#include <cmath>

#include "odd_even_test.h"
#include "point.h"
#include "vec.h"

bool below(point& a, point& b, point& c) {
  long dx = static_cast<long>(a.x) - static_cast<long>(b.x);
  long dy = static_cast<long>(a.y) - static_cast<long>(b.y);
  if (std::abs(dx) <= 1) {
    return static_cast<long>(c.x) < std::max(static_cast<long>(a.x), static_cast<long>(b.x));
  }
  long m = dy / dx;
  long d = static_cast<long>(a.y) - m * static_cast<long>(a.x);
  return static_cast<long>(c.y) < m * static_cast<long>(c.x) + d;
}

bool is_inside(const point& p, const std::vector<point>& polygon) {
  bool inside = false;
  std::size_t n = polygon.size();
  
  for (std::size_t i = 0, j = n - 1; i < n; j = i++) {
    if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
        (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
      inside = !inside;
  }
  
  return inside;
}

bool below(vec& a, vec& b, vec& c) {
  double m = (a.y - b.y) / (a.x - b.x);
  double d = a.y - m * a.x;
  return c.y < m * c.x + d;
}

bool is_inside(vec& p, std::vector<vec>& polygon) {
  bool inside = false;
  for (std::size_t i = 0, j = 1, k = 2, l = 3; i < polygon.size(); ++i,
                   ++j %= polygon.size(), ++k %= polygon.size(),
                   ++l %= polygon.size()) {
    if (((p.x < polygon[i].x && p.x > polygon[j].x) ||
         (p.x > polygon[i].x && p.x < polygon[j].x)) &&
        (below(polygon[i], polygon[j], p)))
      inside = !inside;
    if ((p.x == polygon[j].x) &&
        ((p.x < polygon[i].x && p.x > polygon[k].x) ||
         (p.x > polygon[i].x && p.x < polygon[k].x)) &&
        (p.y < polygon[j].y))
      inside = !inside;
    if ((p.x == polygon[j].x && p.x == polygon[k].x) &&
        (!((p.x < polygon[i].x && p.x < polygon[l].x) ||
           (p.x > polygon[i].x && p.x > polygon[l].x))))
      inside = !inside;
  }
  return inside;
}

bool is_inside(std::vector<point>& polygon, std::vector<point>& inside) {
  point p = *inside.begin();
  return is_inside(p, polygon);
}
