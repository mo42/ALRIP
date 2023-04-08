#ifndef ROT_SQUARE_H_
#define ROT_SQUARE_H_

#include "line.h"
#include "vec.h"

/**
 * Representation of a square that can be arbitrarily rotated.
 *
 * The rotated square is represented by four points which are the
 * corners.
 */
class rot_square {
  /// Adapt the square so that p does not lie inside.
  rot_square adapt(const vec& p, const line& l) const;

public:
  /// Rotation angle of the square
  double angle;
  /// Translation
  vec translation;
  /// Scaling factor
  vec scale;
  /// Size of the square
  double size;
  /// Corner points of the square
  vec corners[4];
  /// Create degenerate square
  rot_square(void);
  /// Copy constructor
  rot_square(const rot_square& s);
  /// Creates square defined by four points (the polygon created by the
  /// four points must equal the square, i.e., the sequence matters).
  rot_square(const vec& p, const vec& q, const vec& r, const vec& s);
  /// Compare squares according to size.
  bool operator>(const rot_square& s) const;
  /// Compare squares according to size.
  bool operator<(const rot_square& s) const;
  /// Assignment operator
  rot_square& operator=(const rot_square& s);
  /// Get the size of the square
  double get_size();
  /// True if the point lies inside the square
  bool inside(const vec& p) const;
  /// True if the segment lies inside the square
  bool inside(const line& l) const;
  /// Returns true if at least one corner is on line l
  bool attached(const line& l) const;
  /// Adapt the square such that l does not lie inside
  void adapt(const line& l);
  /// Set the rotation angle
  void set_angle(double a);
  /// Set the scaling factor
  void set_scale(vec& s);
  /// Get the rotaton angle
  double get_angle(void) const;
  /// Set the translation
  void set_translation(const vec& v);
  /// Return the rectangle of the square by scaling
  void transform_rectangle(void);
  /// Return the i-th transformed corner of the square.
  vec corner(unsigned char i) const;
  /// Transform the point v according to the coordinate system of the
  /// square.
  vec transform(vec& v) const;
  /// Get the middle point of the square
  vec mid_point(void) const;
};

#endif // ROT_SQUARE_H_
