#include <cmath>
#include <vector>

#include "line.h"
#include "math.h"
#include "point.h"
#include "rot_square.h"
#include "vec.h"

double length(const vec& v, const vec& u) {
  vec l = v - u;
  return l.length();
}

line transform(const line& l, const vec& translation, const vec& scale,
               double angle) {
  vec start = transform(l.start(), translation, scale, angle);
  vec target = transform(l.target(), translation, scale, angle);
  return line(start, target);
}

vec transform(const vec& v, const vec& translation, const vec& scale,
              double angle) {
  // Translate
  vec u = v + translation;
  // Rotate CW
  u.rotate_cw(angle);
  // Scale
  u.x *= scale.x;
  u.y *= scale.y;
  // Rotate CCW
  u.rotate_ccw(angle);
  // Translate
  return u - translation;
}

tcoord dist(tcoord i, tcoord j) { return i > j ? i - j : j - i; }

tcoord min(tcoord i, tcoord j) { return i < j ? i : j; }

tcoord max(tcoord i, tcoord j) { return i > j ? i : j; }

bool comp_point_x(point i, point j) { return i.x < j.x; }

bool comp_point_y(point i, point j) { return i.y < j.y; }

double size(rot_square& s) {
  return length(s.corners[1], s.corners[0]) *
         length(s.corners[3], s.corners[0]);
}

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
