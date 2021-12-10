#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <array>
#include <fstream>
#include <map>
#include <cstdint>

namespace rs = ranges;
namespace rv = ranges::views;

uint64_t simulate(std::vector<int> input, int days)
{
    std::array<uint64_t, 9> timers{};

    for (auto t : input) {
        timers[t]++;
    }

    for (int i = 0; i < days; ++i) {
        rs::rotate(timers, timers.begin() + 1);
        timers[6] += timers[8];
    }

    return rs::accumulate(timers, uint64_t(0));
}

size_t part1(std::vector<int> input)
{
    return simulate(input, 80);
}

size_t part2(std::vector<int> input)
{
    return simulate(input, 256);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 06\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = aoc::split_line_by<int>(ifs);

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
    ss << R"(3,4,3,1,2)";

    auto input = aoc::split_line_by<int>(ss);
    REQUIRE(5934 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(3,4,3,1,2)";

    auto input = aoc::split_line_by<int>(ss);
    REQUIRE(26984457539 == part2(input));
}

#endif