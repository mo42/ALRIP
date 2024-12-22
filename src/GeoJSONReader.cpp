#include "empty_box/GeoJSONReader.h"
#include <cstdio>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <stdexcept>

namespace empty_box {

Polygon parse_polygon_from(const std::string& filepath) {
  FILE* file = std::fopen(filepath.c_str(), "r");
  if (!file) {
    throw std::runtime_error("Unable to open file: " + filepath);
  }

  char buffer[65536];
  rapidjson::FileReadStream is(file, buffer, sizeof(buffer));
  rapidjson::Document document;
  document.ParseStream(is);
  std::fclose(file);

  if (document.HasParseError()) {
    throw std::runtime_error("Error parsing JSON: " +
                             std::to_string(document.GetParseError()));
  }

  if (!document.IsObject()) {
    throw std::runtime_error("Root JSON element is not an object");
  }

  if (!document.HasMember("type") || !document["type"].IsString() ||
      std::string(document["type"].GetString()) != "FeatureCollection") {
    throw std::runtime_error("Invalid GeoJSON: Expected FeatureCollection");
  }

  Polygon polygon;

  const auto& features = document["features"];
  if (!features.IsArray()) {
    throw std::runtime_error("Invalid GeoJSON: features should be an array");
  }

  for (const auto& feature : features.GetArray()) {
    if (feature.HasMember("geometry") && feature["geometry"].IsObject()) {
      const auto& geometry = feature["geometry"];
      if (geometry["type"].IsString() &&
          std::string(geometry["type"].GetString()) == "Polygon") {
        const auto& coordinates = geometry["coordinates"];
        if (coordinates.IsArray() && coordinates.Size() > 0) {
          for (const auto& pointArray : coordinates[0].GetArray()) {
            if (pointArray.IsArray() && pointArray.Size() == 2) {
              double x = pointArray[0].GetDouble();
              double y = pointArray[1].GetDouble();
              polygon.push_back(Point(x, y));
            }
          }
          break; // Assuming one polygon for simplicity
        }
      }
    }
  }

  return polygon;
}

} // namespace empty_box
