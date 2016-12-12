#ifndef LINE_H_
#define LINE_H_

#include "vec.h"

/**
 * Represents a segment and a line.
 *
 * This class represents a segment defined by two points and a line
 * which goes through theses two points.
 */
class line {
  /// Start, direction and target vector
  vec s, d, t;
  /// Determinant of the matrix constructed by two vectors.
  static double det(vec s, vec t);

public:
  /// Create a degenerate segment.
  line(void);
  /// Create a copy of l.
  line(const line &l);
  /// Create a segment with s as the start point and t as the endpoint.
  line(const vec &s, const vec &t);
  /// Return the parameter at which this line intersects l
  double intersect(const line &l) const;
  /// Return true if this line intersects l.
  bool does_intersect(line &l) const;
  /// Return the intersection point of this line and l.
  vec intersect_point(line &l) const;
  /// Return true if the intersection point is on this segments.
  bool intersect_segment(const line &l) const;
  /// Return true if the intersection point is on both segments.
  bool intersect_double_segment(const line &l) const;
  /// Return the point on this line of parameter a.
  vec at(double a) const;
  /// Return the start point.
  vec start(void) const;
  /// Return the target point.
  vec target(void) const;
  /// Return the direction vector.
  vec dir(void) const;
  /// Return true if v is one of the endpoints.
  bool endpoint(const vec &v) const;
  /// Return a boolean value depending on the side of v.
  bool side(const vec &v) const;
  /// Return true if both endpoints of l are on the same side.
  bool side(const line &l) const;
  /// Return the length of the segment
  double length(void) const;
  /// Return true if v is a point on the segment
  bool in_segment(const vec &v) const;
};

#endif // LINE_H_
