#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

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
    fmt::print("Advent of Code 2021 - Day XX\n");

    std::ifstream ifs{"puzzle.in"};

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"()";

    REQUIRE(0 == part1());
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
