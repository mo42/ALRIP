#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <stack>
#include <vector>

#include "les.h"
#include "box.h"
#include "corner.h"
#include "math.h"
#include "mesh_point.h"
#include "square.h"
#include "types.h"

/**
 * Return true if no point is inside the square.
 */
bool is_empty(square& s, std::vector<point>& points) {
  for (auto i = points.begin(); i != points.end(); ++i)
    if (s.is_inside(*i))
      return false;
  return true;
}

/**
 * Return the point having the same x and a larger y
 * coordinate and a maximum distance to p.
 */
tcoord look_up(mesh_point& p) { return p.up == nullptr ? p.y : p.up->y; }

/**
 * Return the point having the same x and a lower y
 * coordinate and a maximum distance to p.
 */
tcoord look_down(mesh_point& p) { return p.down == nullptr ? p.y : p.down->y; }

/**
 * Return the largest square given three points such that no one
 * lies inside the square.
 */
square largest_square_triple(point& i, point& j, point& k,
                             std::vector<point>& points, box& b) {
  corner c = corner(i, j, b);
  if (c.is_in_area(k)) {
    square s = c.largest_square(k);
    if (is_empty(s, points))
      return s;
    else
      return square();
  }
  c = corner(j, i, b);
  if (c.is_in_area(k)) {
    square s = c.largest_square(k);
    if (is_empty(s, points))
      return s;
    else
      return square();
  }
  // There are triples that do not define a square.
  return square();
}

/**
 * Return the largest square given a set of points such that no one
 * lies inside the square (brute force).
 */
square largest_square_point_set(std::vector<point>& points, box& b) {
  square t, s;
  for (auto i = points.begin(); i != points.end(); ++i) {
    for (auto j = std::next(i, 1); j != points.end(); ++j) {
      for (auto k = std::next(j, 1); k != points.end(); ++k) {
        t = largest_square_triple(*i, *j, *k, points, b);
        if (t > s)
          s = t;
      }
    }
  }
  return s;
}

/**
 * Add points at the beginning and end of the lists.
 */
std::list<point> add_endpoints(std::vector<point>& list, box& b, tcoord m) {
  if ((*list.begin()).x < m) {
    std::list<point> l;
    for (auto i = list.begin(); i != list.end(); ++i)
      l.push_back(*i);
    return b.endpoints(l, type_side::left);
  } else {
    std::list<point> r;
    for (auto i = list.begin(); i != list.end(); ++i)
      r.push_back(*i);
    return b.endpoints(r, type_side::right);
  }
}

/**
 * Add points to the list if two consecutive points have a larger
 * vertical distance than the square.
 */
void add_distance_points(std::list<point>& points, box& b, square s, tcoord m) {
  if ((*points.begin()).x < m) {
    auto i = points.begin();
    for (auto i_next = std::next(i, 1); i_next != points.end(); ++i, ++i_next) {
      if (dist((*i).y, (*i_next).y) > s.size) {
        point p1 = point(b.left_x(), (*i).y);
        point p2 = point(b.left_x(), (*i_next).y);
        points.insert(i_next, p1);
        points.insert(i_next, p2);
        ++i;
        ++i;
      }
    }
  } else {
    auto i = points.begin();
    for (auto i_next = std::next(i, 1); i_next != points.end(); ++i, ++i_next) {
      if (dist((*i).y, (*i_next).y) > s.size) {
        point p1 = point(b.right_x(), (*i).y);
        point p2 = point(b.right_x(), (*i_next).y);
        points.insert(i_next, p1);
        points.insert(i_next, p2);
        ++i;
        ++i;
      }
    }
  }
}

/**
 * Convert the list to a list of mesh_points and add corner points such
 * that it has an orthogonal shape.
 */
std::list<mesh_point> create_mesh_list(std::list<point>& points, tcoord m) {
  std::list<mesh_point> list;
  // If there are points with the same y-value keep only the nearest
  for (auto i = points.begin(); i != points.end(); ++i) {
    list.push_back(mesh_point((*i).x, (*i).y));
  }
  // Create corner points
  auto j = list.begin();
  if ((*j).x < m) {
    for (auto j_next = std::next(j, 1); j_next != list.end(); ++j, ++j_next) {
      if ((*j).y == (*j_next).y)
        continue;
      if ((*j).x > (*j_next).x) {
        list.insert(j_next, mesh_point((*j_next).x, (*j).y));
        ++j;
      } else if ((*j).x < (*j_next).x) {
        list.insert(j_next, mesh_point((*j).x, (*j_next).y));
        ++j;
      }
    }
  } else {
    for (auto j_next = std::next(j, 1); j_next != list.end(); ++j, ++j_next) {
      if ((*j).y == (*j_next).y)
        continue;
      if ((*j).x < (*j_next).x) {
        list.insert(j_next, mesh_point((*j_next).x, (*j).y));
        ++j;
      } else if ((*j).x > (*j_next).x) {
        list.insert(j_next, mesh_point((*j).x, (*j_next).y));
        ++j;
      }
    }
  }
  return list;
}

/**
 * Add points to the list such that every point has at least one
 * (and at most two) points with the same horizontal distance to m.
 */
void fill_vertical_points(std::list<mesh_point>& list, tcoord m, box& b) {
  if ((*list.begin()).x < m) { // Left side
    std::stack<mesh_point> su;
    auto i = list.begin();
    for (auto i_next = std::next(i, 1); i_next != list.end(); ++i, ++i_next) {
      if (!su.empty() && su.top().x == (*i).x)
        su.pop();
      if ((*i).y == (*i_next).y) {
        if ((*i).x > (*i_next).x) {
          su.push(*i);
        } else {
          while (!su.empty() && su.top().x < (*i_next).x) {
            mesh_point p = su.top();
            su.pop();
            list.insert(i_next, mesh_point(p.x, (*i).y));
            ++i;
          }
        }
      }
    }
    tcoord by = b.upper_y();
    while (!su.empty()) {
      list.push_back(mesh_point(su.top().x, by));
      su.pop();
    }
    std::stack<mesh_point> sd;
    auto j = list.rbegin();
    for (auto j_next = std::next(j, 1); j_next != list.rend(); ++j, ++j_next) {
      if (!sd.empty() && sd.top().x == (*j).x)
        sd.pop();
      if ((*j).y == (*j_next).y) {
        if ((*j).x > (*j_next).x) {
          sd.push(*j);
        } else {
          while (!sd.empty() && sd.top().x < (*j_next).x) {
            mesh_point p = sd.top();
            sd.pop();
            list.insert(j_next.base(), mesh_point(p.x, (*j).y));
            ++j;
            ++j_next;
          }
        }
      }
    }
    by = b.lower_y();
    while (!sd.empty()) {
      list.push_front(mesh_point(sd.top().x, by));
      sd.pop();
    }
  } else { // Right side
    std::stack<mesh_point> su;
    auto i = list.begin();
    for (auto i_next = std::next(i, 1); i_next != list.end(); ++i, ++i_next) {
      if (!su.empty() && su.top().x == (*i).x)
        su.pop();
      if ((*i).y == (*i_next).y) {
        if ((*i).x > (*i_next).x) {
          while (!su.empty() && su.top().x > (*i_next).x) {
            mesh_point p = su.top();
            su.pop();
            list.insert(i_next, mesh_point(p.x, (*i).y));
            ++i;
          }
        } else {
          su.push(*i);
        }
      }
    }
    tcoord by = b.upper_y();
    while (!su.empty()) {
      list.push_back(mesh_point(su.top().x, by));
      su.pop();
    }
    std::stack<mesh_point> sd;
    auto j = list.rbegin();
    for (auto j_next = std::next(j, 1); j_next != list.rend(); ++j, ++j_next) {
      if (!sd.empty() && sd.top().x == (*j).x)
        sd.pop();
      if ((*j).y == (*j_next).y) {
        if ((*j).x > (*j_next).x) {
          while (!sd.empty() && sd.top().x > (*j_next).x) {
            mesh_point p = sd.top();
            sd.pop();
            list.insert(j_next.base(), mesh_point(p.x, (*j).y));
            ++j;
            ++j_next;
          }
        } else {
          sd.push(*j);
        }
      }
    }
    by = b.lower_y();
    while (!sd.empty()) {
      list.push_front(mesh_point(sd.top().x, by));
      sd.pop();
    }
  }
}

/**
 * Remove equivalent consecutive points.
 */
void strip(std::list<mesh_point>& list) {
  auto i = list.begin();
  for (auto i_next = std::next(i, 1); i_next != list.end(); ++i, ++i_next) {
    if ((*i) == (*i_next)) {
      i = list.erase(i);
      ++i_next;
    }
  }
}

/**
 * Add connections between points having the same vertical
 * distance to m.
 */
void add_vertical_connections(std::list<mesh_point>& list, tcoord m) {
  if ((*list.begin()).x < m) {
    // Down connections
    std::stack<mesh_point*> sd;
    auto i = list.begin();
    for (auto i_next = std::next(i, 1); i_next != list.end(); ++i, ++i_next) {
      if ((*i).y == (*i_next).y) {
        if ((*i).x < (*i_next).x) {
          if (!sd.empty()) {
            mesh_point* p = sd.top();
            if (p->down != nullptr) {
              (*i_next).down = p->down;
            } else {
              (*i_next).down = p;
            }
            sd.pop();
          }
        } else {
          sd.push(&*i);
        }
      } else {
        if ((*i).down == nullptr) {
          (*i_next).down = &*i;
        } else {
          (*i_next).down = (*i).down;
        }
      }
    }
    // Up connections
    std::stack<mesh_point*> su;
    auto j = list.rbegin();
    for (auto j_next = std::next(j, 1); j_next != list.rend(); ++j, ++j_next) {
      if ((*j).y == (*j_next).y) {
        if ((*j).x < (*j_next).x) {
          if (!su.empty()) {
            mesh_point* p = su.top();
            if (p->up != nullptr) {
              (*j_next).up = p->up;
            } else {
              (*j_next).up = p;
            }
            su.pop();
          }
        } else {
          su.push(&*j);
        }
      } else {
        if ((*j).up == nullptr) {
          (*j_next).up = &*j;
        } else {
          (*j_next).up = (*j).up;
        }
      }
    }
  } else {
    std::stack<mesh_point*> sd;
    auto i = list.begin();
    // Down connections
    for (auto i_next = std::next(i, 1); i_next != list.end(); ++i, ++i_next) {
      if ((*i).y == (*i_next).y) {
        if ((*i).x > (*i_next).x) {
          if (!sd.empty()) {
            mesh_point* p = sd.top();
            if (p->down != nullptr) {
              (*i_next).down = p->down;
            } else {
              (*i_next).down = p;
            }
            sd.pop();
          }
        } else {
          sd.push(&*i);
        }
      } else {
        if ((*i).down == nullptr) {
          (*i_next).down = &*i;
        } else {
          (*i_next).down = (*i).down;
        }
      }
    }
    // Up connections
    std::stack<mesh_point*> su;
    auto j = list.rbegin();
    for (auto j_next = std::next(j, 1); j_next != list.rend(); ++j, ++j_next) {
      if ((*j).y == (*j_next).y) {
        if ((*j).x > (*j_next).x) {
          if (!su.empty()) {
            mesh_point* p = su.top();
            if (p->up != nullptr) {
              (*j_next).up = p->up;
            } else {
              (*j_next).up = p;
            }
            su.pop();
          }
        } else {
          su.push(&*j);
        }
      } else {
        if ((*j).up == nullptr) {
          (*j_next).up = &*j;
        } else {
          (*j_next).up = (*j).up;
        }
      }
    }
  }
}

/**
 * Add points such that every point on one side of m has a
 * corresponding point (same height) on the other side.
 */
void fill_horizontal_points(std::list<mesh_point>& left,
                            std::list<mesh_point>& right, tcoord m) {
  auto il = left.begin();
  auto il_next = std::next(il, 1);
  auto ir = right.begin();
  auto ir_next = std::next(ir, 1);
  while (il_next != left.end() && ir_next != right.end()) {
    if ((*il).y == (*il_next).y) {
      ++il;
      ++il_next;
      continue;
    }
    if ((*ir).y == (*ir_next).y) {
      ++ir;
      ++ir_next;
      continue;
    }
    if ((*il_next).y == (*ir_next).y) {
      ++ir;
      ++ir_next;
      ++il;
      ++il_next;
      continue;
    }
    if ((*il_next).y < (*ir_next).y) {
      mesh_point p = mesh_point((*ir).x, (*il_next).y);
      right.insert(ir_next, p);
      ++ir;

      ++il;
      ++il_next;
      continue;
    }
    if ((*il_next).y > (*ir_next).y) {
      mesh_point p = mesh_point((*il).x, (*ir_next).y);
      left.insert(il_next, p);
      ++il;

      ++ir;
      ++ir_next;
      continue;
    }
  }
}

/**
 * Add horizontal connections between the points.
 */
void add_horizontal_connections(std::list<mesh_point>& left,
                                std::list<mesh_point>& right) {
  // Left to right connections
  auto il = left.begin();
  auto ir = right.begin();
  while (il != left.end()) {
    mesh_point* max = &*ir;
    while ((*il).y == (*ir).y && ir != right.end()) {
      tcoord d1 = dist((*il).x, (*ir).x);
      tcoord d2 = dist((*il).x, max->x);
      if (d1 > d2)
        max = &*ir;
      ++ir;
    }
    while ((*il).y == max->y) {
      (*il).right = max;
      ++il;
      if (il == left.end())
        break;
    }
  }
  // Right to left connections
  il = left.begin();
  ir = right.begin();
  while (ir != right.end()) {
    mesh_point* max = &*il;
    while ((*ir).y == (*il).y && il != left.end()) {
      tcoord d1 = dist((*ir).x, (*il).x);
      tcoord d2 = dist((*ir).x, max->x);
      if (d1 > d2)
        max = &*il;
      ++il;
    }
    while ((*ir).y == max->y) {
      (*ir).left = max;
      ++ir;
      if (ir == right.end())
        break;
    }
  }
}

/**
 * Return the largest square given a set of points such that no one
 * lies inside the square (divide and conquer).
 */
square largest_square_recursion(std::vector<point>& points_x,
                                std::vector<point>& points_y, box& b) {
  if (points_x.size() < 3) {
    return square();
  } else if (points_x.size() < 100 && points_x.size() >= 3) {
    return largest_square_point_set(points_x, b);
  } else {
    // Calculate median w. r. t. x values
    point p = points_x[points_x.size() / 2];
    tcoord m = p.x;

    std::vector<point> left_x, right_x;
    for (auto i = points_x.begin(); i != points_x.end(); ++i) {
      if ((*i).x < m)
        left_x.push_back(*i);
      else
        right_x.push_back(*i);
    }
    std::vector<point> left_y, right_y;
    for (auto i = points_y.begin(); i != points_y.end(); ++i) {
      if ((*i).x < m)
        left_y.push_back(*i);
      else
        right_y.push_back(*i);
    }
    // Recursion step
    box box_left;
    b.split_left_at(m, box_left);
    box box_right;
    b.split_right_at(m, box_right);
    square square_left = largest_square_recursion(left_x, left_y, box_left);
    square square_right = largest_square_recursion(right_x, right_y, box_right);
    // Merge step
    square max = square_right > square_left ? square_right : square_left;
    std::list<point> left = add_endpoints(left_y, box_left, m);
    std::list<point> right = add_endpoints(right_y, box_right, m);
    add_distance_points(left, box_left, max, m);
    add_distance_points(right, box_right, max, m);
    std::list<mesh_point> ml = create_mesh_list(left, m);
    std::list<mesh_point> mr = create_mesh_list(right, m);
    fill_vertical_points(ml, m, box_left);
    fill_vertical_points(mr, m, box_right);
    fill_horizontal_points(ml, mr, m);
    strip(ml);
    strip(mr);
    add_vertical_connections(mr, m);
    add_vertical_connections(ml, m);
    add_horizontal_connections(ml, mr);
    // Lower left corner largest square
    auto j = mr.begin();
    for (auto i = ml.begin(); i != ml.end(); ++i) {
      tcoord h = dist((*i).x, (*i).right->x);
      tcoord v = dist((*i).y, look_up(*i));
      tcoord s = min(h, v);

      while ((*j).y < (*i).y)
        ++j;
      while ((*j).y <= (*i).y + s) {
        if (look_up(*j) >= (*i).y + s) {
          break;
        }
        ++j;
      }
      s = min(s, dist((*i).x, (*j).x));
      square n = square(*i, s, 0);
      if (n > max)
        max = n;
    }
    // Upper left corner largest square
    auto k = mr.rbegin();
    for (auto i = ml.rbegin(); i != ml.rend(); ++i) {
      tcoord h = dist((*i).x, (*i).right->x);
      tcoord v = dist((*i).y, look_down(*i));
      tcoord s = min(h, v);

      while ((*k).y > (*i).y)
        ++k;
      while ((*k).y >= (*i).y - s) {
        if (look_down(*k) <= (*i).y - s) {
          break;
        }
        ++k;
      }
      s = min(s, dist((*i).x, (*k).x));
      square n = square(*i, s, 3);
      if (n > max)
        max = n;
    }
    // Lower right corner largest square
    j = ml.begin();
    for (auto i = mr.begin(); i != mr.end(); ++i) {
      tcoord h = dist((*i).x, (*i).left->x);
      tcoord v = dist((*i).y, look_up(*i));
      tcoord s = min(h, v);

      while ((*j).y < (*i).y)
        ++j;
      while ((*j).y <= (*i).y + s) {
        if (look_up(*j) >= (*i).y + s) {
          break;
        }
        ++j;
      }
      s = min(s, dist((*i).x, (*j).x));
      square n = square(*i, s, 1);
      if (n > max)
        max = n;
    }

    // Upper right corner largest square
    k = ml.rbegin();
    for (auto i = mr.rbegin(); i != mr.rend(); ++i) {
      tcoord h = dist((*i).x, (*i).left->x);
      tcoord v = dist((*i).y, look_down(*i));
      tcoord s = min(h, v);

      while ((*k).y > (*i).y)
        ++k;
      while ((*k).y >= (*i).y - s) {
        if (look_down(*k) <= (*i).y - s) {
          break;
        }
        ++k;
      }
      s = min(s, dist((*i).x, (*k).x));
      square n = square(*i, s, 2);
      if (n > max)
        max = n;
    }
    return max;
  }
}

square les(std::vector<point>& points) {
  std::vector<point> points_x;
  points_x.reserve(points.size());
  copy(points.begin(), points.end(), back_inserter(points_x));
  std::sort(points_x.begin(), points_x.end(), comp_point_x);
  std::sort(points.begin(), points.end(), comp_point_y);

  tcoord x_max = 0, x_min = UINT_MAX;
  tcoord y_max = 0, y_min = UINT_MAX;
  for (auto i = points.begin(); i != points.end(); ++i) {
    if ((*i).x > x_max)
      x_max = (*i).x;
    if ((*i).x < x_min)
      x_min = (*i).x;
    if ((*i).y > y_max)
      y_max = (*i).y;
    if ((*i).y < y_min)
      y_min = (*i).y;
  }
  box b = box(x_min, x_max, y_max, y_min);
  return largest_square_recursion(points_x, points, b);
}
