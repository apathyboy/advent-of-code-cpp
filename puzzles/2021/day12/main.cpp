#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

std::vector<std::pair<std::string, std::string>> read_input(std::istream& input)
{
    return rs::getlines(input) | rv::transform([](auto&& s) {
               return std::pair{s.substr(0, s.find_first_of('-')), s.substr(s.find_first_of('-') + 1)};
           })
           | rs::to<std::vector>;
}

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
    fmt::print("Advent of Code 2021 - Day 12\n");

    std::ifstream ifs{"puzzle.in"};

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)";

    REQUIRE(0 == part1());
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"()";

    REQUIRE(0 == part2());
}

#endif