#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

#include "reader.h"
#include "vec.h"

/**
 * This function splits the string s at the character d into a
 * vector of strings.
 */
static std::vector<std::string> split(const std::string& s, char d) {
  std::stringstream stream(s);
  std::string i;
  std::vector<std::string> tokens;
  while (std::getline(stream, i, d))
    tokens.push_back(i);
  return tokens;
}

/**
 * Read graph file and returns a vector of parsed_segments.
 */
inner_outer read_file(std::string path) {
  std::ifstream infile(path);
  std::string line;
  unsigned long segmentcount; // nodecount;
  std::getline(infile, line);
  std::vector<std::string> split_line = split(line, ':');
  std::map<unsigned long, vec> m;
  // Collect all nodes and store them in a map
  if (split_line[0].compare("Nodecount") == 0) {
    // nodecount = std::stol(split_line[1]);
    std::getline(infile, line);
    split_line = split(line, ';');
    for (auto i = split_line.begin(); i != split_line.end(); ++i) {
      std::vector<std::string> node = split(*i, ':');
      unsigned long node_id = static_cast<unsigned long>(std::stol(node[0]));
      std::vector<std::string> coord = split(node[1], ',');
      unsigned long y = static_cast<unsigned long>(std::stoi(coord[0]));
      unsigned long x = static_cast<unsigned long>(std::stoi(coord[1]));
      m[node_id] = vec(static_cast<double>(x), static_cast<double>(y));
    }
  }
  std::getline(infile, line);
  split_line = split(line, ':');
  std::map<unsigned long, std::vector<vec>> segments;
  // Collect all segments and store them in a map
  if (split_line[0].compare("Segmentcount") == 0) {
    segmentcount = static_cast<unsigned long>(std::stol(split_line[1]));
    for (size_t i = 0; i < segmentcount; ++i) {
      std::getline(infile, line);
      split_line = split(line, ':');
      unsigned long segment_id =
          static_cast<unsigned long>(std::stol(split(split_line[0], ',')[0]));
      split_line = split(split_line[1], ',');
      std::vector<vec> polygon;
      for (auto j = split_line.begin(); j != split_line.end(); ++j) {
        polygon.push_back(m[static_cast<unsigned long>(std::stol(*j))]);
      }
      segments[segment_id] = polygon;
    }
  }
  // Decide if a segment is of inner or outer type
  std::getline(infile, line);
  split_line = split(line, ':');
  if (split_line[0].compare("Citycount") != 0) {
  }
  std::vector<std::vector<vec>> inner;
  std::vector<std::vector<vec>> outer;
  std::getline(infile, line);
  split_line = split(line, ':');
  if (split_line[0].compare("Areacount") == 0) {
    std::getline(infile, line);
    split_line = split(line, ':');
    split_line = split(split_line[1], ',');
    int outer_count = std::stoi(split_line[0]);
    int inner_count = std::stoi(split_line[1]);
    // Pass over all outer segment ids and collect outer segments
    std::getline(infile, line);
    split_line = split(line, ',');
    for (int i = 0; i < outer_count; ++i) {
      unsigned long segment_id =
          static_cast<unsigned long>(std::stol(split_line[i]));
      outer.push_back(segments[segment_id]);
    }
    // Pass over all inner segment ids and collect inner segments
    std::getline(infile, line);
    split_line = split(line, ',');
    for (int i = 0; i < inner_count; ++i) {
      unsigned long segment_id =
          static_cast<unsigned long>(std::stol(split_line[i]));
      inner.push_back(segments[segment_id]);
    }
  }
  return inner_outer(outer, inner);
}
