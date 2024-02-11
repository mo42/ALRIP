cmake_minimum_required(VERSION 3.6)

project(ler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "clang++")

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_options(
    -Wall
    -Wextra
    -pedantic
    # -Werror until all errors are fixed
    -Wconversion
    -Wsign-conversion
    -Wshadow
    -Wfloat-equal
    -Wmissing-include-dirs
    -Wmissing-declarations
    -Wpointer-arith
    -Wredundant-decls
    -Wuninitialized
    -Wunused
    -Wold-style-cast
    -Woverloaded-virtual
    -Wctor-dtor-privacy
    -Wformat=2
  )
endif()

find_package(RapidJSON REQUIRED)
include_directories(${RapidJSON_INCLUDE_DIRS})

set(SOURCES
    src/adapt.cpp
    src/ap.cpp
    src/box.cpp
    src/corner.cpp
    src/les.cpp
    src/line.cpp
    src/math.cpp
    src/mesh_point.cpp
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
