#ifndef MESH_POINT_H_
#define MESH_POINT_H_

#include "point.h"
#include "types.h"

/**
 * Enhanced point containing pointers.
 */
class mesh_point : public point {
public:
  /// Pointer to a point with the same height and larger x component.
  mesh_point* right;
  /// Pointer to a point with the same height and smaller x component.
  mesh_point* left;
  /// Pointer to a point with the same x component and a larger y
  /// component.
  mesh_point* up;
  /// Pointer to a point with the same x component and a smaller y
  /// component.
  mesh_point* down;
  /// Generate a specific mesh point.
  mesh_point(tcoord x, tcoord y);
};
#endif // MESH_POINT_H_
