add_executable(test_geojson_reader test_geojson_reader.cpp)
target_link_libraries(test_geojson_reader empty_box)
add_test(NAME TestGeoJSONReader COMMAND test_geojson_reader)
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
