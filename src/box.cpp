#include <list>

#include "box.h"
#include "point.h"
#include "types.h"

box::box() {
  this->lx = 0;
  this->ux = 0;
  this->uy = 0;
  this->ly = 0;
}
box::box(tcoord lx, tcoord ux, tcoord uy, tcoord ly) {
  this->lx = lx;
  this->ux = ux;
  this->uy = uy;
  this->ly = ly;
}

tcoord box::distance_x(const point& p, type_side x, type_side y) {
  if (x == type_side::left) {
    return p.x - lx;
  } else {
    return ux - p.x;
  }
}

tcoord box::distance_y(const point& p, type_side x, type_side y) {
  if (y == type_side::right) {
    return p.y - ly;
  } else {
    return uy - p.y;
  }
}

void box::split_left_at(tcoord m, box& b) {
  b.set(lx, m, uy, ly);
  return;
}

void box::split_right_at(tcoord m, box& b) {
  b.set(m, ux, uy, ly);
  return;
}

void box::set(tcoord lx, tcoord ux, tcoord uy, tcoord ly) {
  this->lx = lx;
  this->ux = ux;
  this->uy = uy;
  this->ly = ly;
}

std::list<point> box::endpoints(std::list<point> list, type_side s) {
  if (s == type_side::left) {
    if (!((*list.begin()).x == lx && (*list.begin()).y == ly)) {
      list.push_front(mesh_point(lx, ly));
    }
    if (!(list.back().x == lx && list.back().y == ly)) {
      list.push_back(mesh_point(lx, uy));
    }
    return list;
  } else {
    if (!((*list.begin()).x == ux && (*list.begin()).y == ly)) {
      list.push_front(mesh_point(ux, ly));
    }
    if (!(list.back().x == ux && list.back().y == uy)) {
      list.push_back(mesh_point(ux, uy));
    }
    return list;
  }
}

tcoord box::upper_y() { return uy; }

tcoord box::lower_y() { return ly; }

tcoord box::left_x() { return lx; }

tcoord box::right_x() { return ux; }
