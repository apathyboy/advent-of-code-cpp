
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rv = ranges::views;

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
    fmt::print("Advent of Code 2020 - Day 10\n");

    std::ifstream ifs{"days/day10/input.txt"};

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2_2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 10 problems")
{
    std::stringstream ss;

    ss << R"()";

    SECTION("Can solve part 1 example") { REQUIRE(0 == part1()); }

    SECTION("Can solve part 2 example") { REQUIRE(0 == part2()); }
}

#endif
