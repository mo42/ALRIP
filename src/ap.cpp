#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include <time.h>

#include "libabel.h"
#include "reader.h"
#include "rot_square.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    std::cout << "Usage: filename scale factor" << std::endl;
    return 1;
  } else {
    inner_outer io = read_file(argv[1]);
    int scale = std::stoi(argv[2]);
    rot_square s = approx_max_axis_aligned(io, scale);
    s.corners[0].y /= scale;
    s.corners[1].y /= scale;
    s.corners[2].y /= scale;
    s.corners[3].y /= scale;
    std::ofstream out;
    out.open("result.dat");
    out << static_cast<unsigned int>(s.corners[0].x) << ","
        << static_cast<unsigned int>(s.corners[0].y) << std::endl
        << static_cast<unsigned int>(s.corners[1].x) << ","
        << static_cast<unsigned int>(s.corners[1].y) << std::endl
        << static_cast<unsigned int>(s.corners[2].x) << ","
        << static_cast<unsigned int>(s.corners[2].y) << std::endl
        << static_cast<unsigned int>(s.corners[3].x) << ","
        << static_cast<unsigned int>(s.corners[3].y);
    out.close();
    return 0;
  }
}
