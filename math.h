#ifndef MATH_H_
#define MATH_H_

#include "line.h"
#include "point.h"
#include "square.h"
#include "types.h"
#include "vec.h"

/// Return the distance between point u and v
double length(const vec &v, const vec &u);
/// Transform the line according to the coordinate system of the square,
/// i. e. translation, scaling and rotation.
line transform(const line &l, const vec &translation, const vec &scale,
               double angle);
/// Transform the point according to the coordinate system of the square,
/// i. e. translation, scaling and rotation.
vec transform(const vec &v, const vec &translation, const vec &scale,
              double angle);
/// Return the absolute distance
tcoord dist(tcoord i, tcoord j);
/// Return the minimum
tcoord min(tcoord i, tcoord j);
/// Return the maximum
tcoord max(tcoord i, tcoord j);
/// Return true if the x component of i is smaller than the component
/// of j.
bool comp_point_x(point i, point j);
/// Return true if the y component of i is smaller than the component
/// of j.
bool comp_point_y(point i, point j);
/// Return the size of the the square i. e. the area.
double size(rot_square &s);

#endif // MATH_H_
