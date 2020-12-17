#include <fmt/format.h>
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
    fmt::print("Advent of Code 2020 - Day 23\n");

    std::string input_path = "days/day23/puzzle.in";

    std::ifstream ifs{input_path};

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
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
