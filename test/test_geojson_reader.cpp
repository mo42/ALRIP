#include "empty_box/GeoJSONReader.h"
#include <iostream>
#include <cassert>

int main() {
    try {
        const std::string filepath = "data/sample.geojson";
        auto polygon = empty_box::parse_polygon_from(filepath);
        assert(polygon.size() == 4);
        auto it = polygon.vertices_begin();
        assert(*it++ == empty_box::Point(0.0, 0.0));
        assert(*it++ == empty_box::Point(1.0, 0.0));
        assert(*it++ == empty_box::Point(1.0, 1.0));
        assert(*it++ == empty_box::Point(0.0, 1.0));
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
