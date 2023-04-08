#include "point.h"
#include "types.h"

point::point(void) {
  x = 0;
  y = 0;
}

point::point(tcoord x, tcoord y) {
  this->x = x;
  this->y = y;
}

void point::set(tcoord x, tcoord y) {
  this->x = x;
  this->y = y;
}

point& point::operator=(const point& rhs) {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

bool point::operator>(const point& p) { return x > p.x; }

bool point::operator<(const point& p) { return x < p.x; }

bool point::operator==(const point& s) { return x == s.x && y == s.y; }
