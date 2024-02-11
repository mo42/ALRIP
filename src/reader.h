#include <string>
#include <tuple>
#include <vector>

#include "vec.h"

typedef std::vector<std::vector<vec>> polygons;
typedef std::tuple<std::vector<std::vector<vec>>, std::vector<std::vector<vec>>>
    inner_outer;
typedef std::vector<polygons> instances;

inner_outer read_file(std::string path);
