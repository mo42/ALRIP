#include <algorithm>
#include <cstdlib>

#include "point.h"
#include "rot_square.h"
#include "square.h"

square::square(void) { size = 0; }

square::square(point &p, tcoord size, unsigned char location) : size(size) {
  switch (location) {
  case 0:
    llc = p;
    lrc = point(p.x + size, p.y);
    urc = point(p.x + size, p.y + size);
    ulc = point(p.x, p.y + size);
    break;
  case 1:
    llc = point(p.x - size, p.y);
    lrc = p;
    urc = point(p.x, p.y + size);
    ulc = point(p.x - size, p.y + size);
    break;
  case 2:
    llc = point(p.x - size, p.y - size);
    lrc = point(p.x, p.y - size);
    urc = p;
    ulc = point(p.x - size, p.y);
    break;
  case 3:
    llc = point(p.x, p.y - size);
    lrc = point(p.x + size, p.y - size);
    urc = point(p.x + size, p.y);
    ulc = p;
    break;
  default:
    size = 0;
    break;
  }
}

square::square(square &&s) {
  std::swap(this->llc, s.llc);
  std::swap(this->lrc, s.lrc);
  std::swap(this->urc, s.urc);
  std::swap(this->ulc, s.ulc);
  std::swap(this->size, s.size);
}

square::square(const square &s) {
  this->size = s.size;
  this->llc = s.llc;
  this->lrc = s.lrc;
  this->urc = s.urc;
  this->ulc = s.ulc;
}

square::square(const point &llc, const point &lrc, const point &urc,
               const point &ulc) {
  this->llc = llc;
  this->lrc = lrc;
  this->urc = urc;
  this->ulc = ulc;
  size = lrc.x - llc.x;
}

bool square::is_degenerate(void) { return size == 0; }

bool square::is_inside(const point &t) {
  if (t.x > llc.x && t.x < lrc.x && t.y > llc.y && t.y < ulc.y)
    return true;
  else
    return false;
}

bool square::operator==(const square &s) const { return size == s.size; }

bool square::operator>(const square &s) const { return size > s.size; }

bool square::operator<(const square &s) const { return size < s.size; }

square &square::operator=(const square &s) {
  llc = s.llc;
  lrc = s.lrc;
  urc = s.urc;
  ulc = s.ulc;
  size = s.size;
  return *this;
}

tcoord square::get_size() { return size; }

void square::set(const point &llc, const point &lrc, const point &urc,
                 const point &ulc) {
  this->llc = llc;
  this->lrc = lrc;
  this->urc = urc;
  this->ulc = ulc;
  size = lrc.x - llc.x;
}

point square::mid_point() {
  tcoord x = (lrc.x - llc.x) / 2 + llc.x;
  tcoord y = (ulc.y - llc.y) / 2 + llc.y;
  return point(x, y);
}

rot_square square::convert_rot_square() const {
  vec c[4];
  c[0].x = static_cast<double>(llc.x);
  c[0].y = static_cast<double>(llc.y);

  c[1].x = static_cast<double>(lrc.x);
  c[1].y = static_cast<double>(lrc.y);

  c[2].x = static_cast<double>(urc.x);
  c[2].y = static_cast<double>(urc.y);

  c[3].x = static_cast<double>(ulc.x);
  c[3].y = static_cast<double>(ulc.y);
  return rot_square(c[0], c[1], c[2], c[3]);
}
