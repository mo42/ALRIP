#ifndef LIBABEL_H_
#define LIBABEL_H_

#include "reader.h"
#include "rot_square.h"

rot_square approx_max_axis_aligned(inner_outer& io, int scale);

rot_square approx_max_rotated(inner_outer& io, double scale);

#endif // LIBABEL_H_
