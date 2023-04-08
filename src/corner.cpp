#include <cstdlib>

#include "box.h"
#include "corner.h"
#include "math.h"
#include "point.h"
#include "square.h"

square corner::corner_case(tcoord size) {
  point llc, lrc, urc, ulc;
  // Lower
  if (y == type_side::right) {
    // Left
    if (x == type_side::left) {
      llc.set(cp.x - size, cp.y - size);
      lrc.set(cp.x, cp.y - size);
      urc.set(cp.x, cp.y);
      ulc.set(cp.x - size, cp.y);
      return square(llc, lrc, urc, ulc);
    }
    // Right
    else {
      llc.set(cp.x, cp.y - size);
      lrc.set(cp.x + size, cp.y - size);
      urc.set(cp.x + size, cp.y);
      ulc.set(cp.x, cp.y);
      return square(llc, lrc, urc, ulc);
    }
  }
  // Upper
  else {
    // Right
    if (x == type_side::right) {
      llc.set(cp.x, cp.y);
      lrc.set(cp.x + size, cp.y);
      urc.set(cp.x + size, cp.y + size);
      ulc.set(cp.x, cp.y + size);
      return square(llc, lrc, urc, ulc);
    }
    // Left
    else {
      llc.set(cp.x - size, cp.y);
      lrc.set(cp.x, cp.y);
      urc.set(cp.x, cp.y + size);
      ulc.set(cp.x - size, cp.y + size);
      return square(llc, lrc, urc, ulc);
    }
  }
}

corner::corner(point& p, point& q, box& b) : b(b) {
  this->cp = point(p.x, q.y);
  if (q.x > p.x)
    x = type_side::right;
  else
    x = type_side::left;
  if (q.y > p.y)
    y = type_side::right;
  else
    y = type_side::left;
}

bool corner::is_in_area(point& q) {
  bool b = (q.x >= cp.x && x == type_side::right) ||
           (q.x <= cp.x && x == type_side::left);
  bool c = (q.y >= cp.y && y == type_side::left) ||
           (q.y <= cp.y && y == type_side::right);
  return b && c;
}

square corner::largest_square(point& q) {
  if (!is_in_area(q)) {
    return square();
  } else {
    tcoord size;
    tcoord dx, dy, m;
    // Get minimum distance to the box
    dx = b.distance_x(cp, x, y);
    dy = b.distance_y(cp, x, y);
    m = dx < dy ? dx : dy;
    // The point is closer to the vertical line
    if (dist(cp.x, q.x) <= dist(cp.y, q.y)) {
      size = dist(cp.y, q.y);
      return corner_case(m > size ? size : m);
    }
    // The point is closer to the horizontal line
    else {
      size = dist(cp.x, q.x);
      return corner_case(m > size ? size : m);
    }
  }
}
