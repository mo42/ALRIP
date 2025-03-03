#ifndef SQUARE_H_
#define SQUARE_H_

#include "point.h"
#include "rot_square.h"

/** \brief Representation of a square
 *
 * A square is represented by four points which are the corners.
 */
class square {
public:
  /// Size of the square
  tcoord size;
  /// Lower left corner
  point llc;
  /// Lower right corner
  point lrc;
  /// Upper right corner
  point urc;
  /// Upper left corner
  point ulc;
  /// Create degenerate square
  square(void);
  /// Create square using one specific point and the size
  square(point& p, tcoord size, unsigned char location);
  /// Move constructor
  square(square&& s);
  /// Copy constructor
  square(const square& s);
  /// Create square by all points
  square(const point& llc, const point& lrc, const point& urc,
         const point& ulc);
  /// True if it represents a degenerate square
  bool is_degenerate(void);
  /// True if the point lies inside the square
  bool is_inside(const point& t);
  /// Return true if two squares are equal.
  bool operator==(const square& s) const;
  /// Compare squares according to size.
  bool operator>(const square& s) const;
  /// Compare squares according to size.
  bool operator<(const square& s) const;
  /// Assignment operator
  square& operator=(const square& s);
  /// Return size of the square
  tcoord get_size();
  /// Set corners of the square
  void set(const point& llc, const point& lrc, const point& urc,
           const point& ulc);
  /// Get the middle point of the square
  point mid_point();
  /// Convert the square to a rot_square
  rot_square convert_rot_square(void) const;
};
#endif // SQUARE_H_
