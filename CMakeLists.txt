cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-shadow-field-in-constructor -Wno-c++98-compat -O3")

project(ler)

include(FetchContent)
FetchContent_Declare(
        rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG 476ffa2fd272243275a74c36952f210267dc3088   # 2023.08.26
)

FetchContent_GetProperties(rapidjson)
if (NOT rapidjson_POPULATED)
    FetchContent_Populate(rapidjson)
endif ()

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

target_include_directories(rr PRIVATE ${rapidjson_SOURCE_DIR}/include)
target_include_directories(ap PRIVATE ${rapidjson_SOURCE_DIR}/include)

