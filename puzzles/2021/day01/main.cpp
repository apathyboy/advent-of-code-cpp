
#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <cstdint>
#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

int64_t accumulate_depth_increases(auto&& r)
{
    return rs::accumulate(
        r | rv::sliding(2) | rv::transform([](auto&& i) { return i[0] < i[1] ? 1 : 0; }),
        0);
}

int64_t part1(const std::vector<int>& input)
{
    return accumulate_depth_increases(input);
}

int64_t part2(const std::vector<int>& input)
{
    return accumulate_depth_increases(
        input | rv::sliding(3) | rv::transform([](auto&& i) { return rs::accumulate(i, 0); }));
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 01\n");

    auto input = aoc::read_int_per_line(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(199
200
208
210
200
207
240
269
260
263)";

    auto input = aoc::read_int_per_line(std::move(ss));

    REQUIRE(7 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(199
200
208
210
200
207
240
269
260
263)";

    auto input = aoc::read_int_per_line(std::move(ss));

    REQUIRE(5 == part2(input));
}

#endif