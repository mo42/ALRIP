#include <cmath>

#include "vec.h"

vec::vec() {
  x = 0.0;
  y = 0.0;
}

vec::vec(const double x, const double y) : x(x), y(y) {}

bool vec::operator==(const vec& v) const { return x == v.x && y == v.y; }

bool vec::operator!=(const vec& v) { return x != v.x || y != v.y; }

vec& vec::operator=(const vec& v) {
  x = v.x;
  y = v.y;
  return *this;
}

vec vec::operator-(void) const { return vec(-x, -y); }

vec vec::operator+(const vec& v) const { return vec(x + v.x, y + v.y); }

vec vec::operator-(const vec& v) const { return vec(x - v.x, y - v.y); }

vec vec::operator*(double s) const {
  vec tmp = *this;
  return tmp *= s;
}

vec vec::operator/(double s) const {
  vec tmp = *this;
  return tmp /= s;
}

vec& vec::operator+=(const vec& v) {
  x += v.x;
  y += v.y;
  return *this;
}

vec& vec::operator-=(const vec& v) {
  x -= v.x;
  y -= v.y;
  return *this;
}

vec& vec::operator*=(double s) {
  x *= s;
  y *= s;
  return *this;
}

vec& vec::operator/=(double s) {
  x /= s;
  y /= s;
  return *this;
}

double vec::operator*(const vec& v) const { return x * v.x + y * v.y; }

double vec::length() const { return sqrt(x * x + y * y); }

vec vec::perpendicular() const { return vec(-y, x); }

void vec::normalize() {
  x /= length();
  y /= length();
}

void vec::rotate_cw(double angle) {
  double tx, ty;
  tx = x * cos(angle) + y * sin(angle);
  ty = -x * sin(angle) + y * cos(angle);
  x = tx;
  y = ty;
}

void vec::rotate_ccw(double angle) {
  double tx, ty;
  tx = x * cos(angle) - y * sin(angle);
  ty = x * sin(angle) + y * cos(angle);
  x = tx;
  y = ty;
}
