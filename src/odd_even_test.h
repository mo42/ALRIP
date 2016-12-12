#include <vector>

#include "point.h"
#include "vec.h"

/**
 * Return true if the point c lies below the segment a b.
 * Point c lies between a and b.
 */
bool below(point &a, point &b, point &c);

/**
 * Return if the point lies inside the polygon.
 * There are at most two subsequent points p1, p2 such that p1.x ==
 * p2.x or p1.y == p2.y
 */
bool is_inside(point &p, std::vector<point> &polygon);

/**
 * Return if the point c lies below the segment a b.
 * Point c lies between a and b.
 */
bool below(vec &a, vec &b, vec &c);

/**
 * Return if the point lies inside the polygon.
 * There are at most two subsequent points p1, p2 such that p1.x ==
 * p2.x or p1.y == p2.y
 */
bool is_inside(vec &p, std::vector<vec> &polygon);

/**
 * Return true if the polygon inside lies inside polygon
 * polygon.
 * The polygons do not intersect.
 */
bool is_inside(std::vector<point> &polygon, std::vector<point> &inside);

