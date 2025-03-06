#include <climits>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <time.h>

#include "adapt.h"
#include "les.h"
#include "math.h"
#include "odd_even_test.h"
#include "point.h"
#include "reader.h"
#include "rot_square.h"
#include "size.h"
#include "square.h"
#include "types.h"

/**
 * \brief Strip segments if there are several consecutive parallel
 * segments.
 *
 * This procedure ensures that there are at most two subsequent
 * points p1, p2 such that p1.x == p2.x or p1.y == p2.y.
 * If there are three or more points only the start and the end
 * are kept which does not change the polygon.
 *
 */
void strip_segments(std::vector<point>& polygon) {
  auto i = polygon.begin();
  auto j = std::next(i, 1);
  for (auto k = std::next(j, 1); k != polygon.end(); ++i, ++j, ++k) {
    if (((*i).x == (*j).x && (*j).x == (*k).x) ||
        ((*i).y == (*j).y && (*j).y == (*k).y)) {
      j = polygon.erase(j);
      ++k;
      ++i;
    }
  }
}

/**
 * \brief Create an instance for the largest empty square algorithm.
 */
std::vector<point> create_instance(polygons p) {
  std::vector<point> points;
  for (auto i = p.begin(); i != p.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      points.push_back(point(static_cast<unsigned long>((*j).x),
                             static_cast<unsigned long>((*j).y)));
    }
  }
  return points;
}

/**
 * \brief Scale all points vertically.
 */
void scale_y(instances& inst, int scale) {
  for (auto i = inst.begin(); i != inst.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      for (auto k = (*j).begin(); k != (*j).end(); ++k) {
        (*k).y *= static_cast<double>(scale);
      }
    }
  }
}

rot_square approx_max_axis_aligned(inner_outer& io, int scale) {
  polygons outer = std::get<0>(io);
  std::cout << "Outer: " << outer.size() << std::endl;
  polygons inner = std::get<1>(io);
  std::cout << "Inner: " << inner.size() << std::endl;
  instances inst;
  // Loop over all outer polygons
  for (auto i = outer.begin(); i != outer.end(); ++i) {
    polygons k;
    k.push_back(*i);
    // For every inner polygon, check if it is inside of *i
    for (auto j = inner.begin(); j != inner.end(); ++j) {
      vec p = (*j)[0];
      if (is_inside(p, (*i)))
        k.push_back(*j);
    }
    inst.push_back(k);
  }
  if (scale <= 1)
    scale = 1;
  scale_y(inst, scale);
  // Loop over all instances
  rot_square s, rs;
  unsigned int max_size = 0;
  for (auto i = inst.begin(); i != inst.end(); ++i) {
    unsigned int new_size = 0;
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      max_size += (*j).size();
    }
    max_size = std::max(new_size, max_size);

    std::vector<point> points = create_instance(*i);
    strip_segments(points);
    while (true) {
      square n = les(points);
      // Adapt square to segments
      rs = n.convert_rot_square();
      if (rs.size == 0.0)
        break;
      vec m = rs.mid_point();
      // Check if the midpoint is inside the outer polygon
      if (is_inside(m, (*i)[0])) {
        // Adapt all segments
        for (auto j = (*i).begin(); j != (*i).end(); ++j)
          adapt(rs, *j);
        break;
      }
      point pm =
          point(static_cast<unsigned int>(m.x), static_cast<unsigned int>(m.y));
      points.push_back(pm);
    }
    if (rs > s) {
      s = rs;
    }
  }
  return s;
}

typedef std::tuple<vec, vec> vec_pair;

double length(vec_pair pair) {
  return length(std::get<0>(pair), std::get<1>(pair));
}

/**
 * \brief Return the pair with minimal distance.
 */
vec_pair min_pair(line& l, line& m) {
  vec_pair pairs[4];
  pairs[0] = vec_pair(l.start(), m.start());
  pairs[1] = vec_pair(l.start(), m.target());
  pairs[2] = vec_pair(l.target(), m.start());
  pairs[3] = vec_pair(l.target(), m.target());
  size_t min = 0;
  for (size_t i = 0; i < 4; ++i)
    if (length(pairs[i]) < length(pairs[min]))
      min = i;
  return pairs[min];
}

/**
 * \brief Return the distance of the minimal pair.
 */
double min_pair_dist(line& l, line& m) { return length(min_pair(l, m)); }

/**
 * \brief Largest square constraint by 3 fixed points
 * \post return largest square
 * \post attachment of segments is guaranteed by construction
 */
rot_square fixed_points_construction(line& l, line& m, line& n) {
  line l1, l2;
  line top;
  if (min_pair_dist(l, m) > min_pair_dist(l, n)) {
    if (min_pair_dist(l, m) > min_pair_dist(m, n)) {
      l1 = l;
      l2 = m;
      top = n;
    } else {
      l1 = m;
      l2 = n;
      top = l;
    }
  } else {
    if (min_pair_dist(l, n) > min_pair_dist(m, n)) {
      l1 = l;
      l2 = n;
      top = m;
    } else {
      l1 = m;
      l2 = n;
      top = l;
    }
  }
  vec_pair pair = min_pair(l1, l2);
  vec d = std::get<0>(pair) - std::get<1>(pair);
  vec perpd = d.perpendicular();
  line t1 = line(std::get<0>(pair), std::get<0>(pair) + d.perpendicular());
  line t2 = line(std::get<1>(pair), std::get<1>(pair) + d.perpendicular());
  if (top.intersect_segment(t1) && top.intersect_segment(t2)) {
    vec i1 = t1.intersect_point(top);
    vec i2 = t2.intersect_point(top);
    line top_line = line(i1, i1 + d);
    if (top_line.side(i2) == top_line.side(std::get<0>(pair)))
      top_line = line(i2, i2 + d);
    i1 = t1.intersect_point(top_line);
    if (t1.side(i1 + d) != t1.side(std::get<1>(pair)))
      d *= -1.0;
    if (top_line.side(i1 + perpd) != top_line.side(std::get<0>(pair)))
      perpd *= -1.0;
    return rot_square(i1, i1 + perpd, i1 + perpd + d, i1 + d);
  } else if (top.intersect_segment(t1)) {
    vec i = t1.intersect_point(top);
    vec p;
    if (t1.side(top.start()) == t1.side(std::get<1>(pair))) {
      p = top.start();
    } else {
      p = top.target();
    }
    line top_line = line(i, i + d);
    if (top_line.side(p) == top_line.side(std::get<0>(pair)))
      top_line = line(p, p + d);
    i = t1.intersect_point(top_line);
    if (t1.side(i + d) != t1.side(std::get<1>(pair)))
      d *= -1.0;
    if (top_line.side(i + perpd) != top_line.side(std::get<0>(pair)))
      perpd *= -1.0;
    return rot_square(i, i + perpd, i + perpd + d, i + d);
  } else if (top.intersect_segment(t2)) {
    vec i = t2.intersect_point(top);
    vec p;
    if (t2.side(top.start()) == t2.side(std::get<0>(pair))) {
      p = top.start();
    } else {
      p = top.target();
    }
    line top_line = line(i, i + d);
    if (top_line.side(p) == top_line.side(std::get<0>(pair)))
      top_line = line(p, p + d);
    i = t2.intersect_point(top_line);
    if (t2.side(i + d) != t2.side(std::get<0>(pair)))
      d *= -1.0;
    if (top_line.side(i + perpd) != top_line.side(std::get<1>(pair)))
      perpd *= -1.0;
    return rot_square(i, i + perpd, i + perpd + d, i + d);
  } else {
    if (t1.side(top.start()) == t1.side(t2.start()) &&
        t2.side(top.start()) == t2.side(t1.start())) {
      line top_line = line(top.start(), top.start() + d);
      if (top_line.side(top.target()) == top_line.side(std::get<0>(pair)))
        top_line = line(top.target(), top.target() + d);
      vec i1 = t1.intersect_point(top_line);
      if (t1.side(i1 + d) != t1.side(std::get<1>(pair)))
        d *= -1.0;
      if (top_line.side(i1 + perpd) != top_line.side(std::get<0>(pair)))
        perpd *= -1.0;
      return rot_square(i1, i1 + perpd, i1 + perpd + d, i1 + d);
    } else {
      return rot_square();
    }
  }
}

/**
 * \brief Largest square constraint by 3 fixed points
 *
 * \post Return largest square
 * \post Attachment of segments is guaranteed by construction
 * \post Square contains no segments
 */
rot_square fixed_points(line& l, line& m, line& n) {
  rot_square s = fixed_points_construction(l, m, n);
  if (s.inside(l.start()) || s.inside(l.target()) || s.inside(m.start()) ||
      s.inside(m.target()) || s.inside(n.start()) || s.inside(n.target()))
    s.size = 0.0;
  if (s.inside(l) || s.inside(m) || s.inside(n))
    s.size = 0.0;
  return s;
}

bool exists_other_side(line& s, vec p, line& l) {
  return s.side(p) != s.side(l.start()) || s.side(p) != s.side(l.target());
}

/**
 * \brief Largest square parallel to line l
 *
 * \pre p is on the right side of the corner
 * \pre Corner has right angle
 *
 * \post Return largest square
 * \post Attachment to p is guaranteed by construction
 */
rot_square corner(line& c1, line& c2, vec p) {
  vec i = c1.intersect_point(c2);
  line l1 = line(p, p + c1.dir());
  line l2 = line(p, p + c2.dir());
  vec i21 = l2.intersect_point(c1);
  vec i12 = l1.intersect_point(c2);
  if (length(p, i12) > length(p, i21)) {
    vec d = i12 - p;
    d = d.perpendicular();
    if (c1.side(i + d) != c1.side(p))
      d *= -1.0;
    return rot_square(i, i21, i21 + d, i + d);
  } else {
    vec d = i21 - p;
    d = d.perpendicular();
    if (c2.side(i + d) != c2.side(p))
      d *= -1.0;
    return rot_square(i, i12, i12 + d, i + d);
  }
}

/**
 * \brief Largest square parallel to line l
 *
 * \pre l and s are a corner
 * \pre n is on both sides of s
 * \pre s_side is the side of the corner
 *
 * \post Return largest square
 * \post Attachment to n is guaranteed by construction
 */
rot_square corner_overlap(line& l, line& s, line& n, bool s_side) {
  vec i = l.intersect_point(s);
  vec j = s.intersect_point(n);
  vec d = s.dir().perpendicular();
  if (s.side(i + d) != s_side)
    d *= -1.0;
  vec p;
  if (s.side(n.start()) == s_side)
    p = n.start();
  else
    p = n.target();
  line pl = line(j, j + l.dir());
  if (pl.side(p) == pl.side(l.start())) {
    pl = line(p, p + l.dir());
    j = s.intersect_point(pl);
  }
  d = i - j;
  d = d.perpendicular();
  if (s.side(i + d) != s.side(p))
    d *= -1.0;
  return rot_square(i, j, j + d, i + d);
}

/**
 * \brief Largest square parallel to line l
 *
 * \pre s is on the right side of the corner
 *
 * \post Return largest square
 * \post Attachment to s is guaranteed by construction
 *
 */
rot_square corner(line& c1, line& c2, line& s) {
  rot_square ss = corner(c1, c2, s.start());
  rot_square st = corner(c1, c2, s.target());
  if (ss.inside(s.target()) || ss.inside(s))
    ss.size = 0.0;
  if (st.inside(s.start()) || st.inside(s))
    st.size = 0.0;
  vec d1 = c1.dir();
  vec i = c1.intersect_point(c2);
  if (c2.side(i + d1) != c2.side(s.start()))
    d1 *= -1.0;
  vec d2 = c2.dir();
  if (c1.side(i + d2) != c1.side(s.start()))
    d2 *= -1.0;
  line d = line(i, i + d1 + d2);
  double a = s.intersect(d);
  rot_square sd;
  if (a >= 0.0 && a <= 1.0) {
    vec j = s.intersect_point(d);
    line t = line(j, j + d1);
    vec p = t.intersect_point(c2);
    t = line(j, j + d2);
    vec q = t.intersect_point(c1);
    sd = rot_square(i, p, j, q);
  }
  if (sd.inside(s.start()) || sd.inside(s.target()) || sd.inside(s))
    sd.size = 0.0;
  return std::max(std::max(ss, st), sd);
}

/**
 * \brief Largest square parallel to line l
 *
 * \pre m and n are on the same side of l
 *
 * \post Return largest square
 * \post Zero square if l, m or n aren't attached to the square
 */
rot_square single_side(line& l, line& m, line& n) {
  vec d = l.dir().perpendicular();
  line s1 = line(m.start(), m.start() + d);
  line s2 = line(m.target(), m.target() + d);
  rot_square sq1, sq2;
  // Handle s1
  if (exists_other_side(s1, m.target(), l) &&
      exists_other_side(s1, m.target(), n)) {
    if (s1.side(n) && s1.side(m.target()) != s1.side(n.start()))
      sq1 = corner(l, s1, n);
    else
      sq1 = corner_overlap(l, s1, n, !s1.side(m.target()));
  }
  // Handle s2
  if (exists_other_side(s2, m.start(), l) &&
      exists_other_side(s2, m.start(), n)) {
    if (s2.side(n) && s2.side(m.start()) != s2.side(n.start()))
      sq2 = corner(l, s2, n);
    else
      sq2 = corner_overlap(l, s2, n, !s1.side(m.start()));
  }
  if (!sq1.attached(l) || !sq1.attached(m) || !sq1.attached(n))
    sq1.size = 0.0;
  if (!sq2.attached(l) || !sq2.attached(m) || !sq2.attached(n))
    sq2.size = 0.0;
  return sq1 > sq2 ? sq1 : sq2;
}

/**
 * \brief Largest square parallel to line l
 *
 * \pre m is on both sides of l
 * \pre n is on one side of l
 *
 * \post Return largest square
 * \post Attachment to n is guaranteed
 */
rot_square single_overlap(line& l, line& m, line& n) {
  vec i = l.intersect_point(m);
  vec d = l.dir().perpendicular();
  line s = line(i, i + d);
  vec p;
  if (l.side(m.start()) == l.side(n.start()))
    p = m.start();
  else
    p = m.target();
  if (s.side(p) == s.side(l.start()))
    s = line(p, p + d);
  if (s.side(l.start()) == s.side(n.start()) && s.side(n))
    return corner(l, s, n);
  else if (s.side(l.start()) == s.side(n.start()) ||
           s.side(l.start()) == s.side(n.target()))
    return corner_overlap(l, s, n, s.side(l.start()));
  return rot_square();
}

/**
 * \brief Largest square parallel to line l
 *
 * \pre m and n are on both sides of l
 * \pre l is between m and n
 *
 * \post Return largest square
 * \post l, m and n are attached to the square
 */
rot_square double_overlap(line& l, line& m, line& n) {
  const vec i1 = l.intersect_point(m);
  const vec i2 = l.intersect_point(n);
  vec s1, s2, s3, s4;
  if (l.side(m.start()) == l.side(n.start())) {
    s1 = m.start();
    s3 = n.start();

    s2 = m.target();
    s4 = n.target();
  } else {
    s1 = m.start();
    s3 = n.target();

    s2 = m.target();
    s4 = n.start();
  }
  const vec d = l.dir().perpendicular();
  line l1 = line(i1, i1 + d);
  if (l1.side(s1) == l1.side(s3))
    l1 = line(s1, s1 + d);
  line l2 = line(i2, i2 + d);
  if (l2.side(s3) == l2.side(s1))
    l2 = line(s3, s3 + d);
  vec i1t = l.intersect_point(l1);
  vec i2t = l.intersect_point(l2);
  vec s = i1t - i2t;
  s = s.perpendicular();
  if (l.side(i1t + s) != l.side(s1))
    s *= -1.0;
  rot_square sq1 = rot_square(i1t, i1t + s, i2t + s, i2t);

  l1 = line(i1, i1 + d);
  if (l1.side(s2) == l1.side(s4))
    l1 = line(s2, s2 + d);
  l2 = line(i2, i2 + d);
  if (l2.side(s4) == l2.side(s2))
    l2 = line(s4, s4 + d);
  i1t = l.intersect_point(l1);
  i2t = l.intersect_point(l2);
  s = i1t - i2t;
  s = s.perpendicular();
  if (l.side(i1t + s) != l.side(s2))
    s *= -1.0;
  rot_square sq2 = rot_square(i1t, i1t + s, i2t + s, i2t);
  if (!sq1.attached(l) || !sq1.attached(m) || !sq1.attached(n))
    sq1.size = 0.0;
  if (!sq2.attached(l) || !sq2.attached(m) || !sq2.attached(n))
    sq2.size = 0.0;
  return sq1 > sq2 ? sq1 : sq2;
}

/**
 * \brief Largest square parallel to line l
 *
 * \post Return largest square
 * \post l, m and n are attached to the square
 *
 */
rot_square sliding_point(line& l, line& m, line& n) {
  if (l.side(m.start()) == l.side(n.start()) && l.side(m) && l.side(n))
    return single_side(l, m, n);
  else if (l.side(m.start()) != l.side(n.start()) && l.side(m) && l.side(n))
    return rot_square();
  else if (l.side(m.start()) != l.side(m.target()) && l.side(n))
    return single_overlap(l, m, n);
  else if (l.side(m.start()) != l.side(m.target()) &&
           l.side(n.start()) != l.side(n.target()))
    return double_overlap(l, m, n);
  else
    return rot_square();
}

/**
 * \brief Largest square in triangle
 *
 * \pre l, m, n are a triangle
 *
 * \post Return largest square
 */
rot_square sliding_points(line& bc, line& ba, line& ac) {
  vec a = ba.intersect_point(ac);
  vec b = bc.intersect_point(ba);
  vec c = bc.intersect_point(ac);
  vec dbc = b - c;
  dbc = dbc.perpendicular();
  line l1 = line(b, b + dbc);
  line l2 = line(c, c + dbc);
  if (l1.side(a) != l2.side(a)) {
    if (bc.side(b + dbc) == bc.side(a))
      dbc *= -1.0;
    l1 = line(a, b + dbc);
    l2 = line(a, c + dbc);
    vec c1 = l1.intersect_point(bc);
    vec c2 = l2.intersect_point(bc);
    vec s = c1 - c2;
    s = s.perpendicular();
    if (bc.side(b + s) != bc.side(a))
      s *= -1.0;
    return rot_square(c1, c1 + s, c2 + s, c2);
  } else {
    return rot_square();
  }
}

/**
 * \brief Return the largest square for one specific permutation of l, m
 * and n.
 */
rot_square largest_square(line& l, line& m, line& n, vec& scale) {
  // Transform according to l
  double angle = acos(l.dir().x / l.dir().length());
  if (l.dir().y < 0.0)
    angle = -angle;
  vec translation = -l.start();
  line nl = transform(l, translation, scale, angle);
  line nm = transform(m, translation, scale, angle);
  line nn = transform(n, translation, scale, angle);
  rot_square sp = sliding_point(nl, nm, nn);
  // Save transform within the square
  sp.set_angle(angle);
  sp.set_scale(scale);
  sp.set_translation(translation);
  // Transform according to d
  line l1, l2;
  line top;
  if (min_pair_dist(l, m) > min_pair_dist(l, n)) {
    if (min_pair_dist(l, m) > min_pair_dist(m, n)) {
      l1 = l;
      l2 = m;
      top = n;
    } else {
      l1 = m;
      l2 = n;
      top = l;
    }
  } else {
    if (min_pair_dist(l, n) > min_pair_dist(m, n)) {
      l1 = l;
      l2 = n;
      top = m;
    } else {
      l1 = m;
      l2 = n;
      top = l;
    }
  }
  vec_pair pair = min_pair(l1, l2);
  vec d = std::get<0>(pair) - std::get<1>(pair);
  double angle_fixed = acos(d.x / d.length());
  if (d.y < 0.0)
    angle_fixed = -angle_fixed;
  vec translation_fixed = -std::get<0>(pair);
  line fl = transform(l, translation_fixed, scale, angle_fixed);
  line fm = transform(m, translation_fixed, scale, angle_fixed);
  line fn = transform(n, translation_fixed, scale, angle_fixed);
  // Save transform within the square
  rot_square fp = fixed_points(fl, fm, fn);
  fp.set_angle(angle_fixed);
  fp.set_scale(scale);
  fp.set_translation(translation_fixed);
  if (!fp.attached(fl) || !fp.attached(fm) || !fp.attached(fn))
    fp.size = 0.0;
  // Transform according to l
  // Save transform within the square
  rot_square sps = sliding_points(nl, nm, nn);
  sps.set_angle(angle);
  sps.set_scale(scale);
  sps.set_translation(translation);
  if (!sps.attached(nl) || !sps.attached(nm) || !sps.attached(nn))
    sps.size = 0.0;
  return std::max(std::max(sps, sp), fp);
}

/**
 * \brief Return the largest square for all permutations of l, m and n.
 */
rot_square largest_square_permutation(line& l, line& m, line& n, vec& scale) {
  rot_square max = largest_square(l, m, n, scale);
  max = std::max(max, largest_square(l, n, m, scale));
  max = std::max(max, largest_square(m, l, n, scale));
  max = std::max(max, largest_square(m, n, l, scale));
  max = std::max(max, largest_square(n, l, m, scale));
  return std::max(max, largest_square(n, m, l, scale));
}

/**
 * \brief Sample the polygon once and return the largest square.
 */
rot_square sample_square(std::vector<std::vector<vec>>& polygon, vec& scale) {
  // Sample uniformly at random from a multi-polygon
  // Calculate the size of the entire instance
  unsigned int size = 0;
  for (auto i = polygon.begin(); i != polygon.end(); ++i)
    size += (*i).size();
  // Generate sample select (sub-) polygon
  unsigned int sample = rand() % size;
  // select (sub-) polygon
  unsigned int l = 0, u = 0;
  line l0;
  for (auto i = polygon.begin(); i != polygon.end(); ++i) {
    u += (*i).size();
    if (l <= sample && sample < u) {
      unsigned int j = sample - l;
      l0 = line((*i)[j], (*i)[j + 1 % (*i).size()]);
    }
    l += (*i).size();
  }
  sample = rand() % size;
  l = 0;
  u = 0;
  line l1;
  for (auto i = polygon.begin(); i != polygon.end(); ++i) {
    u += (*i).size();
    if (l <= sample && sample < u) {
      unsigned int j = sample - l;
      l1 = line((*i)[j], (*i)[j + 1 % (*i).size()]);
    }
    l += (*i).size();
  }
  sample = rand() % size;
  l = 0;
  u = 0;
  line l2;
  for (auto i = polygon.begin(); i != polygon.end(); ++i) {
    u += (*i).size();
    if (l <= sample && sample < u) {
      unsigned int j = sample - l;
      l2 = line((*i)[j], (*i)[j + 1 % (*i).size()]);
    }
    l += (*i).size();
  }
  return largest_square_permutation(l0, l1, l2, scale);
}

/**
 * \brief Return true if the point c lies below the segment a b.
 * \pre Point c lies between a and b.
 */
bool below(vec& a, vec& b, vec& c) {
  long m = (a.y - b.y) / (a.x - b.x);
  long d = a.y - m * a.x;
  return c.y < m * c.x + d;
}

/**
 * \brief Return if the point lies inside the polygon.
 * \pre There are at most two subsequent points p1, p2 such that p1.x ==
 * p2.x or p1.y == p2.y
 */
bool is_inside(vec& p, std::vector<vec>& polygon) {
  bool inside = false;
  for (unsigned int i = 0, j = 1, k = 2, l = 3; i < polygon.size(); ++i,
                    ++j %= polygon.size(), ++k %= polygon.size(),
                    ++l %= polygon.size()) {
    if (((p.x < polygon[i].x && p.x > polygon[j].x) ||
         (p.x > polygon[i].x && p.x < polygon[j].x)) &&
        (below(polygon[i], polygon[j], p)))
      inside = !inside;
    if ((p.x == polygon[j].x) &&
        ((p.x < polygon[i].x && p.x > polygon[k].x) ||
         (p.x > polygon[i].x && p.x < polygon[k].x)) &&
        (p.y < polygon[j].y))
      inside = !inside;
    if ((p.x == polygon[j].x && p.x == polygon[k].x) &&
        (!((p.x < polygon[i].x && p.x < polygon[l].x) ||
           (p.x > polygon[i].x && p.x > polygon[l].x))))
      inside = !inside;
  }
  return inside;
}

/**
 * \brief Return if the square lies inside the polygon.
 * \pre There are at most two subsequent points p1, p2 such that p1.x ==
 * p2.x or p1.y == p2.y
 */
bool inside(std::vector<vec>& polygon, rot_square& s) {
  std::vector<vec> polygon_transf;
  for (auto i = polygon.begin(); i != polygon.end(); ++i) {
    polygon_transf.push_back(s.transform(*i));
  }
  return is_inside(s.corners[0], polygon_transf) &&
         is_inside(s.corners[1], polygon_transf) &&
         is_inside(s.corners[2], polygon_transf) &&
         is_inside(s.corners[3], polygon_transf);
}

/**
 * \brief Sample the polygon k times and return the largest square.
 */
rot_square loop(unsigned int k, std::vector<std::vector<vec>>& polygon,
                double scale) {
  rot_square max;
  std::setw(2);
  std::setprecision(2);
  for (unsigned int i = 0; i < k; ++i) {
    std::cout << (float)i / (float)k * 100 << "\r\r\r\r\r";
    vec s = vec(1.0, scale);
    rot_square s1 = sample_square(polygon, s);
    s.x = scale;
    s.y = 1.0;
    rot_square s2 = sample_square(polygon, s);
    if (s1 > max) {
      s1 = adapt(s1, polygon);
      if (s1 > max)
        if (inside(polygon[0], s1))
          max = std::max(max, s1);
    }
    if (s2 > max) {
      s2 = adapt(s2, polygon);
      if (s2 > max)
        if (inside(polygon[0], s2))
          max = std::max(max, s2);
    }
  }
  return max;
}

rot_square approx_max_rotated(inner_outer& io, double scale) {
  srand(time(NULL));
  polygons outer = std::get<0>(io);
  polygons inner = std::get<1>(io);
  instances inst;
  // Loop over all outer polygons
  for (auto i = outer.begin(); i != outer.end(); ++i) {
    polygons k;
    k.push_back(*i);
    // For every inner polygon, check if it is inside of *i
    for (auto j = inner.begin(); j != inner.end(); ++j) {
      vec p = (*j)[0];
      if (is_inside(p, (*i)))
        k.push_back(*j);
    }
    inst.push_back(k);
  }
  rot_square s;
  for (auto i = inst.begin(); i != inst.end(); ++i) {
    unsigned int size = 0;
    for (auto j = (*i).begin(); j != (*i).end(); ++j)
      size += (*j).size();
    rot_square n = loop(size * size, *i, scale);
    if (n > s) {
      s = n;
    }
  }
  s.transform_rectangle();
  return s;
}

