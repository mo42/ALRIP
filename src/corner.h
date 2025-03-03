#ifndef CORNER_H_
#define CORNER_H_

#include "box.h"
#include "point.h"
#include "square.h"
#include "types.h"

/**
 * \brief Representation of a axis-parallel corner.
 *
 * A corner is defined by two points such that the corner can be
 * restricted by a third point and yields a maximum square.
 */
class corner {
  /// Box which contains the points.
  box b;
  /// Point of the corner
  point cp;
  /// Orientation of the corner around the corner point.
  type_side x, y;
  square corner_case(tcoord size);

public:
  /// Create a corner where p defines the x value and q the y value
  corner(point& p, point& q, box& b);
  /// Check if a point is in the area defined by the corner
  bool is_in_area(point& q);
  /// Return largest square (degenerate square if q is not in area)
  square largest_square(point& q);
};

#endif // CORNER_H_
