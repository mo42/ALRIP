#ifndef LES_H_
#define LES_H_

#include "point.h"
#include "square.h"

/**
 * Calculate the largest empty square from the point set points.
 */
square les(std::vector<point> &points);

#endif // LES_H_
