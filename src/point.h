#ifndef POINT_H_
#define POINT_H_

#include "types.h"

/**
 * \brief Represents a point in two-dimensional euclidian space.
 */
class point {
public:
  /// Generate a point at the origin.
  point(void);
  /// Generate a specific point.
  point(tcoord x, tcoord y);
  /// Set coordinates of a point.
  void set(tcoord x, tcoord y);
  /// Assignment operator
  point& operator=(const point& rhs);
  tcoord x;
  tcoord y;
  /// Return true if the x component of this point is greater than the
  /// component of s.
  bool operator>(const point& s);
  /// Return true if the x component of this point is smaller than the
  /// component of s.
  bool operator<(const point& s);
  /// Return true if the points are equal
  bool operator==(const point& s);
};

#endif // POINT_H_
