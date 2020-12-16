#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

int64_t part1()
{
    return 0;
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 15\n");

    std::string input_path = "days/day15/puzzle.in";

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 15 problems")
{
    std::stringstream ss;

    ss << R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)";

    SECTION("Can solve part 1 example") { REQUIRE(165 == part1()); }

    SECTION("Can solve part 2 example") { REQUIRE(208 == part2()); }
}

#endif
