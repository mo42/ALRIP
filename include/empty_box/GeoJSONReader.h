#ifndef EMPTY_BOX_GEOJSONREADER_H
#define EMPTY_BOX_GEOJSONREADER_H

#include <CGAL/Polygon_2.h>
#include <CGAL/Simple_cartesian.h>
#include <string>
#include <utility>
#include <vector>

namespace empty_box {

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_2 Point;
typedef CGAL::Polygon_2<Kernel> Polygon;

Polygon parse_polygon_from(const std::string& filepath);

} // namespace empty_box

#endif // EMPTY_BOX_GEOJSONREADER_H
