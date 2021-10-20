
#include <fmt/core.h>

int part1()
{
    return 0;
}

int part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2019 - Day 03\n");

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#endif