#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>

#include "reader.h"

/*
 * Return the size of a multi-polygon
 */
double polygon_area(polygons p) {
  double size = 0;
  for (unsigned int i = 0; i < p.size(); ++i) {
    std::ofstream out;
    out.open("polygon.dat");
    for (unsigned int j = 0; j < p[i].size(); ++j) {
      out << p[i][j].x << "," << p[i][j].y << std::endl;
    }
    out.close();
    if (system("./size.py") != 0)
      return -1.0;
    std::ifstream infile("size.dat");
    std::string line;
    std::getline(infile, line);
    if (i == 0)
      size += std::stod(line);
    else
      size -= std::stod(line);
    infile.close();
  }
  return size;
}
