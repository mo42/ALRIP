#include <vector>

#include "odd_even_test.h"
#include "point.h"
#include "vec.h"

bool below(point& a, point& b, point& c) {
  long m = (static_cast<long>(a.y) - static_cast<long>(b.y)) / (static_cast<long>(a.x) - static_cast<long>(b.x));
  long d = static_cast<long>(a.y) - m * static_cast<long>(a.x);
  return static_cast<long>(c.y) < m * static_cast<long>(c.x) + d;
}

bool is_inside(point& p, std::vector<point>& polygon) {
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

bool below(vec& a, vec& b, vec& c) {
  long m = (a.y - b.y) / (a.x - b.x);
  long d = a.y - m * a.x;
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
