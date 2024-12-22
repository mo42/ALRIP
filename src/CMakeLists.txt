add_library(empty_box STATIC GeoJSONReader.cpp)
target_include_directories(empty_box PUBLIC ${CMAKE_SOURCE_DIR}/include)
