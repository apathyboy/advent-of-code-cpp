cmake_minimum_required(VERSION 3.16)

project(advent-of-code-cpp)

find_package(Catch2 CONFIG REQUIRED)

find_package(fmt CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(range-v3 CONFIG REQUIRED)

add_library(aoc include/aoc/aoc.hpp src/aoc.cpp)

add_library(esb::aoc ALIAS aoc)

include(CTest)
include(Catch)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

target_include_directories(
    aoc
    PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/aoc ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(aoc PUBLIC cxx_std_20)

target_compile_options(
    aoc
    PUBLIC $<$<CXX_COMPILER_ID:MSVC>:-W4
           -WX
           -analyze
           -wd4702
           -wd6285>) # TODO range-v3 error

target_link_libraries(aoc PRIVATE range-v3::meta)

add_subdirectory(puzzles)
