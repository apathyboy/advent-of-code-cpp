cmake_minimum_required(VERSION 3.16)

project(advent-of-code-cpp)

find_package(Catch2 CONFIG REQUIRED)

find_package(fmt CONFIG REQUIRED)

find_package(glm CONFIG REQUIRED)

find_package(range-v3 CONFIG REQUIRED)

include(CTest)
include(Catch)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# aoc library

add_library(
    aoc
    include/aoc/aoc.hpp
    include/aoc/intcode.hpp
    src/aoc.cpp
    src/intcode.cpp)

add_library(esb::aoc ALIAS aoc)

target_include_directories(
    aoc
    PUBLIC $<INSTALL_INTERFACE:include> $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/aoc ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(aoc PUBLIC cxx_std_23)

target_compile_options(
    aoc
    PUBLIC $<$<CXX_COMPILER_ID:MSVC>:-W4
           -WX
           -analyze
           -wd4702
           -wd6285>) # TODO range-v3 error

target_link_libraries(aoc PRIVATE glm::glm range-v3::meta)

# aoc unit tests

add_executable(aoc_tests src/aoc_tests.cpp src/intcode_tests.cpp)

target_link_libraries(aoc_tests PRIVATE aoc Catch2::Catch2 fmt::fmt)

target_compile_options(
    aoc_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4201
            -wd4996
            -wd28278
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

catch_discover_tests(aoc_tests)

add_subdirectory(puzzles)
