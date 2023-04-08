#ifndef ADAPT_H_
#define ADAPT_H_

#include <vector>

#include "rot_square.h"
#include "vec.h"

/**
 * Adapt the square to all segments of a polygon.
 */
rot_square adapt(rot_square& s, std::vector<std::vector<vec>>& polygon);
/**
 * Adapt the square to all segments of a multi polygon.
 */
rot_square adapt(rot_square& s, std::vector<vec>& polygon);

#endif // ADAPT_H_
