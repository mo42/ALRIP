#include "mesh_point.h"
#include "point.h"
#include "types.h"

mesh_point::mesh_point(tcoord x, tcoord y) : point(x, y) {
  this->up = nullptr;
  this->down = nullptr;
  this->left = nullptr;
  this->right = nullptr;
}

