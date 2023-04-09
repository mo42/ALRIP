#include <algorithm>
#include <cmath>
#include <limits>
#include <utility>

#include "line.h"
#include "vec.h"

line::line() : s(), d(), t() {}

line::line(const vec& s, const vec& t) {
  this->s = s;
  this->t = t;
  this->d = t - s;
}

double line::intersect(const line& l) const {
  double det_m = det(l.dir(), -d);
  double det_r = det(l.dir(), s - l.start());
  return det_r / det_m;
}

bool line::does_intersect(line& l) const {
  double det_m = det(l.dir(), -d);
  double det_r = det(l.dir(), s - l.start());
  return det_m == 0.0 && det_r != 0.0;
}

vec line::intersect_point(line& l) const {
  double a = intersect(l);
  return at(a);
}

bool line::intersect_segment(const line& l) const {
  double a = intersect(l);
  if (0.0 <= a && a <= 1.0)
    return true;
  else
    return false;
}

bool line::intersect_double_segment(const line& l) const {
  return intersect_segment(l) && l.intersect_segment(*this);
}

vec line::at(double a) const { return vec(s + d * a); }

vec line::start() const { return s; }

vec line::target() const { return t; }

vec line::dir() const { return d; }

double line::det(vec s, vec t) { return s.x * t.y - s.y * t.x; }

bool line::endpoint(const vec& v) const { return v == s || v == t; }

bool line::side(const vec& v) const { return 0 < det(t - s, v - s); }

bool line::side(const line& l) const {
  return side(l.start()) == side(l.target());
}

double line::length(void) const {
  vec v = s - t;
  return v.length();
}

bool line::in_segment(const vec& v) const {
  vec sv = v - s;
  vec tv = v - t;
  if (fabs(det(sv, tv)) <= std::numeric_limits<double>::epsilon() * 64) {
    if (v.x >= std::min(s.x, t.x) && v.x <= std::max(s.x, t.x) &&
        v.y >= std::min(s.y, t.y) && v.y <= std::max(s.y, t.y))
      return true;
    else
      return false;
  }
  return false;
}
