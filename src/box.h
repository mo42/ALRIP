#ifndef BOX_H_
#define BOX_H_

#include <list>

#include "mesh_point.h"
#include "point.h"

/**
 * Represents the box which contains all points.
 *
 * The maximum empty square problem requires a minimal box containing
 * all points.
 */
class box {
  /// Lower x coordinate of the box.
  tcoord lx;
  /// Upper x coordinate of the box.
  tcoord ux;
  /// Upper y coordinate of the box.
  tcoord uy;
  /// Lower y coordinate of the box.
  tcoord ly;

public:
  /// Create degenerate box.
  box(void);
  /// Set borders of the box.
  box(tcoord lx, tcoord rx, tcoord uy, tcoord ly);
  /// Copy constructor
  box(const box &b);
  /// Return the horizontal distance from the point to the box.
  tcoord distance_x(const point &p, type_side x, type_side y);
  /// Return the vertical distance from the point corner to the box.
  tcoord distance_y(const point &p, type_side x, type_side y);
  /// Split the box such that m is the upper coordinate.
  void split_left_at(tcoord m, box &b);
  /// Split the box such that m is the lower x coordinate.
  void split_right_at(tcoord m, box &b);
  /// Set the coordinates of the box.
  void set(tcoord lx, tcoord rx, tcoord uy, tcoord ly);
  /// Add points to the list such that the first and last points
  /// in the list are the corners of the box.
  std::list<point> endpoints(std::list<point> list, type_side s);
  /// Return upper y coordinate.
  tcoord upper_y(void);
  /// Return lower y coordinate.
  tcoord lower_y(void);
  /// Return left y coordinate.
  tcoord left_x(void);
  /// Return right y coordinate.
  tcoord right_x(void);
};

#endif // BOX_H_

