#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include <time.h>

#include "libabel.h"
#include "rot_square.h"
#include "vec.h"

int main(int argc, char** argv) {
  if (argc != 3) {
    return 1;
  } else {
    double scale = 1.0 / std::stoi(argv[2]);
    inner_outer io = read_file(argv[1]);
    rot_square s = approx_max_rotated(io, scale);
    std::ofstream out;
    out.open("result.dat");
    out << static_cast<unsigned int>(s.corners[0].x) << ","
        << static_cast<unsigned int>(s.corners[0].y) << std::endl
        << static_cast<unsigned int>(s.corners[1].x) << ","
        << static_cast<unsigned int>(s.corners[1].y) << std::endl
        << static_cast<unsigned int>(s.corners[2].x) << ","
        << static_cast<unsigned int>(s.corners[2].y) << std::endl
        << static_cast<unsigned int>(s.corners[3].x) << ","
        << static_cast<unsigned int>(s.corners[3].y) << std::endl;
    out.close();
  }
}
