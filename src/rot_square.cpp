#include <cmath>
#include <vector>

#include "line.h"
#include "math.h"
#include "rot_square.h"
#include "vec.h"

rot_square::rot_square(void) { size = 0; }

rot_square::rot_square(const rot_square& s) {
  size = s.size;
  angle = s.angle;
  translation = s.translation;
  scale = s.scale;
  corners[0] = s.corners[0];
  corners[1] = s.corners[1];
  corners[2] = s.corners[2];
  corners[3] = s.corners[3];
}

rot_square::rot_square(const vec& p, const vec& q, const vec& r, const vec& s) {
  corners[0] = p;
  corners[1] = q;
  corners[2] = r;
  corners[3] = s;
  size = length(corners[0], corners[1]);
  angle = .0;
  translation = vec();
  scale = vec(1.0, 1.0);
}

bool rot_square::operator>(const rot_square& s) const { return size > s.size; }

bool rot_square::operator<(const rot_square& s) const { return size < s.size; }

rot_square& rot_square::operator=(const rot_square& s) {
  size = s.size;
  angle = s.angle;
  translation = s.translation;
  scale = s.scale;
  corners[0] = s.corners[0];
  corners[1] = s.corners[1];
  corners[2] = s.corners[2];
  corners[3] = s.corners[3];
  return *this;
}

double rot_square::get_size() { return size; }

bool rot_square::inside(const vec& p) const {
  line l0 = line(corners[0], corners[1]);
  line l1 = line(corners[1], corners[2]);
  line l2 = line(corners[2], corners[3]);
  line l3 = line(corners[3], corners[0]);
  if (l0.side(p) == l1.side(p) && l0.side(p) == l2.side(p) &&
      l0.side(p) == l3.side(p))
    return true;
  else
    return false;
}

bool rot_square::inside(const line& l) const {
  line l0 = line(corners[0], corners[1]);
  line l1 = line(corners[1], corners[2]);
  line l2 = line(corners[2], corners[3]);
  line l3 = line(corners[3], corners[0]);
  if ((l0.intersect_double_segment(l) && l1.intersect_double_segment(l)) ||
      (l0.intersect_double_segment(l) && l2.intersect_double_segment(l)) ||
      (l0.intersect_double_segment(l) && l3.intersect_double_segment(l)) ||
      (l1.intersect_double_segment(l) && l2.intersect_double_segment(l)) ||
      (l1.intersect_double_segment(l) && l3.intersect_double_segment(l)) ||
      (l2.intersect_double_segment(l) && l3.intersect_double_segment(l)))
    return true;
  else
    return false;
}

bool rot_square::attached(const line& l) const {
  line l0 = line(corners[0], corners[1]);
  line l1 = line(corners[1], corners[2]);
  line l2 = line(corners[2], corners[3]);
  line l3 = line(corners[3], corners[0]);
  if (l.in_segment(corners[0]) || l.in_segment(corners[1]) ||
      l.in_segment(corners[2]) || l.in_segment(corners[3]) ||
      l0.in_segment(l.start()) || l0.in_segment(l.target()) ||
      l1.in_segment(l.start()) || l1.in_segment(l.target()) ||
      l2.in_segment(l.start()) || l2.in_segment(l.target()) ||
      l3.in_segment(l.start()) || l3.in_segment(l.target()))
    return true;
  else
    return false;
}

rot_square rot_square::adapt(const vec& p, const line& l) const {
  rot_square max;
  for (unsigned char i = 0; i < 4; ++i) {
    line l1 = line(corners[i], corners[(i + 1) % 4]);
    line l2 = line(corners[i], corners[(i + 3) % 4]);
    line li1 = line(p, p + l2.dir());
    line li2 = line(p, p + l1.dir());
    vec i1 = l1.intersect_point(li1);
    vec i2 = l2.intersect_point(li2);
    vec s;
    if (length(p, i2) > length(p, i1)) {
      s = p - i2;
      s = s.perpendicular();
    } else {
      s = p - i1;
    }
    vec sp = s.perpendicular();
    if (l1.side(l1.start() + s) != l1.side(p))
      s *= -1.0;
    if (l2.side(l2.start() + sp) != l2.side(p))
      sp *= -1.0;
    rot_square sn = rot_square(corners[i], corners[i] + s, corners[i] + s + sp,
                               corners[i] + sp);
    if (sn.inside(l.target()) || sn.inside(l))
      sn.size = 0.0;
    max = std::max(max, sn);
  }
  return max;
}

void rot_square::adapt(const line& l) {
  if (inside(l) || inside(l.start()) || inside(l.target())) {
    rot_square max;
    for (unsigned char i = 0; i < 4; ++i) {
      line d = line(corners[i], corners[(i + 2) % 4]);
      if (l.intersect_segment(d)) {
        vec ip = l.intersect_point(d);
        vec hd = (ip - corners[i]) * 0.5;
        vec hdp = hd.perpendicular();
        rot_square s = rot_square(corners[i], corners[i] + hd + hdp, ip,
                                  corners[i] + hd - hdp);
        if (s.inside(l.start()) || s.inside(l.target()))
          s.size = 0.0;
        max = std::max(max, s);
      }
    }
    if (inside(l.start())) {
      max = std::max(max, adapt(l.start(), l));
    }
    if (inside(l.target())) {
      max = std::max(max, adapt(l.target(), l));
    }
    for (unsigned char i = 0; i < 4; ++i)
      corners[i] = max.corners[i];
    size = max.size;
  }
}

void rot_square::set_angle(double a) { angle = a; }

void rot_square::set_scale(vec& s) { scale = s; }

double rot_square::get_angle() const { return angle; }

void rot_square::set_translation(const vec& v) { translation = v; }

void rot_square::transform_rectangle() {
  scale.x = 1.0 / scale.x;
  scale.y = 1.0 / scale.y;

  corners[0] = ::transform(corners[0], translation, scale, angle);
  corners[1] = ::transform(corners[1], translation, scale, angle);
  corners[2] = ::transform(corners[2], translation, scale, angle);
  corners[3] = ::transform(corners[3], translation, scale, angle);
}

vec rot_square::transform(vec& v) const {
  return ::transform(v, translation, scale, angle);
}

vec rot_square::corner(unsigned char i) const {
  return ::transform(corners[i], translation, scale, angle);
}

vec rot_square::mid_point() const {
  vec d = corners[2] - corners[0];
  d *= 0.5;
  return corners[0] + d;
}
