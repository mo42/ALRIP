cmake_minimum_required(VERSION 3.2)

project(ler)

set(SOURCES
    src/adapt.cpp
    src/ap.cpp
    src/box.cpp
    src/corner.cpp
    src/les.cpp
    src/line.cpp
    src/math.cpp
    src/mesh_point.cpp
    src/odd_even_test.cpp
    src/point.cpp
    src/reader.cpp
    src/rot_square.cpp
    src/rr.cpp
    src/square.cpp
    src/vec.cpp
)

add_executable(rr
               src/rr.cpp
               src/reader.cpp
               src/vec.cpp
               src/line.cpp
               src/rot_square.cpp
               src/math.cpp
               src/adapt.cpp
)

add_executable(ap
               src/ap.cpp
               src/reader.cpp
               src/odd_even_test.cpp
               src/les.cpp
               src/point.cpp
               src/square.cpp
               src/rot_square.cpp
               src/line.cpp
               src/corner.cpp
               src/box.cpp
               src/mesh_point.cpp
               src/math.cpp
               src/vec.cpp
               src/adapt.cpp
)

include_directories(src)
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-shadow-field-in-constructor -Wno-c++98-compat -O3")
