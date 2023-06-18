#include <vector>

#include "odd_even_test.h"
#include "vec.h"

bool is_inside(vec& p, std::vector<vec>& polygon) {
  bool inside = false;
  std::size_t n = polygon.size();
  for (std::size_t i = 0, j = n - 1; i < n; j = i++) {
    if (((polygon[i].y > p.y) != (polygon[j].y > p.y)) &&
        (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x))
      inside = !inside;
  }
  return inside;
}
