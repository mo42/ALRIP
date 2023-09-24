#include <fstream>
#include <iostream>
#include <map>
#include <rapidjson/document.h>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "reader.h"
#include "vec.h"

/**
 * Read GeoJSON file and returns a vector of parsed_segments.
 */
inner_outer read_geo_json(std::string path) {
  std::ifstream file(path);
  std::string json_string((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
  rapidjson::Document document;
  document.Parse(json_string.c_str());
  // Check if valid GeoJSON format
  if (!document.IsObject() || !document.HasMember("type") ||
      std::string(document["type"].GetString()) != "Polygon") {
    std::cout << "Invalid GeoJSON format. Expecting Polygon." << std::endl;
  }
  const rapidjson::Value& polygons = document["coordinates"];
  if (!polygons.IsArray()) {
    std::cout << "No polygons found in the GeoJSON file." << std::endl;
  }
  std::vector<vec> outer;
  std::vector<std::vector<vec>> inners;

  for (rapidjson::SizeType i = 0; i < polygons.Size(); ++i) {
    const rapidjson::Value& polygon = polygons[i];
    if (!polygon.IsArray()) {
      std::cout << "No polygon found in the GeoJSON file." << std::endl;
    }
    std::vector<vec> inner;
    for (rapidjson::SizeType j = 0; j < polygon.Size(); ++j) {
      const rapidjson::Value& coordinate = polygon[j];
      vec v = vec(coordinate[0].GetDouble(), coordinate[1].GetDouble());
      if (i == 0) {
        outer.push_back(v);
      } else {
        inner.push_back(v);
      }
    }
    if (inner.size() > 0) {
      inners.push_back(inner);
    }
  }
  std::vector<std::vector<vec>> outer_boxed = {outer};
  return inner_outer(outer_boxed, inners);
}
