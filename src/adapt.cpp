#include <vector>

#include "line.h"
#include "math.h"
#include "rot_square.h"
#include "vec.h"

rot_square adapt(rot_square &s, std::vector<vec> &polygon) {
  for (unsigned int i = 0, j = 1; i < polygon.size();
       ++i, ++j %= polygon.size()) {
    line l = line(polygon[i], polygon[j]);
    s.adapt(transform(l, s.translation, s.scale, s.angle));
  }
  return s;
}

rot_square adapt(rot_square &s, std::vector<std::vector<vec>> &polygon) {
  for (auto i = polygon.begin(); i != polygon.end(); ++i) {
    adapt(s, *i);
  }
  return s;
}

