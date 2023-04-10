#ifndef VEC_H_
#define VEC_H_

/**
 * Represents a point in two-dimensional Euclidean space and a
 * vector.
 */
class vec {
public:
  double x;
  double y;
  vec(vec const&) = default;
  /// Generate a point at the origin (zero vector).
  vec(void);
  /// Generate a specific point or vector.
  vec(double x, double y);
  /// Return true if two points or vectors are equal.
  bool operator==(const vec& v) const;
  /// Return true if two points or vectors are not equal.
  bool operator!=(const vec& v);
  /// Assignment operator
  vec& operator=(const vec& v);
  /// Invert a vector.
  vec operator-(void) const;
  /// Add a vector to another vector.
  vec operator+(const vec& v) const;
  /// Subtract a vector from another vector.
  vec operator-(const vec& v) const;
  /// Scale a vector.
  vec operator*(double s) const;
  /// Scale a vector.
  vec operator/(double s) const;
  /// Add a vector to another vector.
  vec& operator+=(const vec& v);
  /// Subtract a vector from another vector.
  vec& operator-=(const vec& v);
  /// Scale a vector.
  vec& operator*=(double s);
  /// Scale a vector.
  vec& operator/=(double s);
  /// Return the dot product of two vectors.
  double operator*(const vec& v) const;
  /// Return the Euclidean distance from the origin the point (length of
  /// the vector).
  double length(void) const;
  /// Return a vector that is perpendicular to this vector.
  vec perpendicular(void) const;
  /// Scale the components such that the length equals one.
  void normalize(void);
  /// Rotate the point counterclockwise around the origin.
  void rotate_ccw(double angle);
  /// Rotate the point clockwise around the origin.
  void rotate_cw(double angle);
};

#endif // VEC_H_
