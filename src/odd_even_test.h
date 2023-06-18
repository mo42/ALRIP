#include <vector>

#include "point.h"
#include "vec.h"

/**
 * Return if the point lies inside the polygon.
 * There are at most two subsequent points p1, p2 such that p1.x ==
 * p2.x or p1.y == p2.y
 */
bool is_inside(vec& p, std::vector<vec>& polygon);
