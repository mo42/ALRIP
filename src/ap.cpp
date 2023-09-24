#include <limits>
#include <climits>
#include <cmath>
#include <fstream>
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
#include "point.h"
#include "reader.h"
#include "rot_square.h"
#include "square.h"
#include "types.h"

/**
 * Strip segments if there are several consecutive parallel
 * segments.
 *
 * This procedure ensures that there are at most two subsequent
 * points p1, p2 such that p1.x == p2.x or p1.y == p2.y.
 * If there are three or more points only the start and the end
 * are kept, which does not change the polygon.
 *
 */
static void strip_segments(std::vector<point>& polygon) {
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
 * Create an instance for the largest empty square algorithm.
 */
static std::vector<point> create_instance(polygons p, int scale) {
  std::vector<point> points;
  vec running_max(
      std::numeric_limits<double>::lowest(),
      std::numeric_limits<double>::lowest()
  );
  vec running_min(
      std::numeric_limits<double>::max(),
      std::numeric_limits<double>::max()
  );
  for (auto i = p.begin(); i != p.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      if((*j).x > running_max.x) {
        running_max.x = (*j).x;
      }
      if((*j).y > running_max.y) {
        running_max.y = (*j).y;
      }
      if((*j).x < running_min.x) {
        running_min.x = (*j).x;
      }
      if((*j).y < running_min.y) {
        running_min.y = (*j).y;
      }
    }
  }
  unsigned long tcoord_min = std::numeric_limits<tcoord>::lowest();
  unsigned long tcoord_max = std::numeric_limits<tcoord>::max();
  double scale_x = static_cast<double>(tcoord_max - tcoord_min)
    / (running_max.x - running_min.x)
    / 2;
  double scale_y = static_cast<double>(tcoord_max - tcoord_min)
    / (running_max.y - running_min.y)
    / 2;
  for (auto i = p.begin(); i != p.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      tcoord x = static_cast<tcoord>(((*j).x - running_min.x) * scale_x + tcoord_max);
      tcoord y = static_cast<tcoord>(((*j).y - running_min.y) * scale_y + tcoord_max);
      points.push_back(point(x, y));
    }
  }
  return points;
}

/**
 * Scale all points vertically.
 */
static void scale_y(instances& inst, int scale) {
  for (auto i = inst.begin(); i != inst.end(); ++i) {
    for (auto j = (*i).begin(); j != (*i).end(); ++j) {
      for (auto k = (*j).begin(); k != (*j).end(); ++k) {
        (*k).y *= static_cast<double>(scale);
      }
    }
  }
}

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: filename scale factor" << std::endl;
    return 1;
  } else {
    inner_outer io = read_geo_json(argv[1]);
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
    int scale = std::stoi(argv[2]);
    scale = std::max(scale, 1);
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

      std::vector<point> points = create_instance(*i, scale);
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
        point pm = point(static_cast<unsigned int>(m.x),
                         static_cast<unsigned int>(m.y));
        points.push_back(pm);
      }
      if (rs > s) {
        s = rs;
      }
    }
    s.corners[0].y /= scale;
    s.corners[1].y /= scale;
    s.corners[2].y /= scale;
    s.corners[3].y /= scale;
    std::cout << static_cast<unsigned int>(s.corners[0].x) << ","
              << static_cast<unsigned int>(s.corners[0].y) << std::endl
              << static_cast<unsigned int>(s.corners[1].x) << ","
              << static_cast<unsigned int>(s.corners[1].y) << std::endl
              << static_cast<unsigned int>(s.corners[2].x) << ","
              << static_cast<unsigned int>(s.corners[2].y) << std::endl
              << static_cast<unsigned int>(s.corners[3].x) << ","
              << static_cast<unsigned int>(s.corners[3].y) << std::endl;
    return 0;
  }
}
