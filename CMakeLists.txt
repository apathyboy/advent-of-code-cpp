cmake_minimum_required(VERSION 3.16)

project(aoc2020cpp)

find_package(
    Catch2
    CONFIG
    REQUIRED)

find_package(
    fmt
    CONFIG
    REQUIRED)

find_package(
    range-v3
    CONFIG
    REQUIRED)

add_library(
    aoc2020
    include/aoc2020/aoc2020.hpp
    src/aoc2020.cpp)

add_library(
    esb::aoc2020
    ALIAS
    aoc2020)

include(CTest)
include(Catch)

target_include_directories(
    aoc2020
    PUBLIC $<INSTALL_INTERFACE:include>
           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/aoc2020
            ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_compile_features(aoc2020 PUBLIC cxx_std_20)

target_compile_options(
    aoc2020
    PUBLIC $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:-Wall
           -Wextra
           -Werror>
           $<$<CXX_COMPILER_ID:MSVC>:-W4
           -WX
           -analyze>)

target_link_libraries(aoc2020 PRIVATE range-v3::meta)

add_subdirectory(days)
add_subdirectory(tests)
