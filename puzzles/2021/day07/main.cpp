#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

int part1(std::vector<int> input)
{
    int fuel = std::numeric_limits<int>::max();

    for (int test_pos : input | rv::unique) {
        auto tmp = rs::accumulate(
            input | rv::transform([test_pos](int pos) { return std::abs(test_pos - pos); }),
            0);

        if (tmp < fuel) fuel = tmp;
    }

    return fuel;
}

int part2(std::vector<int> input)
{
    double mean = rs::accumulate(input, 0.0) / input.size();

    int fuel = std::numeric_limits<int>::max();

    for (int test_pos : rv::closed_iota((int)std::floor(mean), (int)std::ceil(mean))) {
        auto tmp = rs::accumulate(
            input | rv::transform([test_pos](int pos) {
                auto tmp = rs::accumulate(rv::closed_iota(0, std::abs(test_pos - pos)), 0);
                return tmp;
            }),
            0);

        if (tmp < fuel) { fuel = tmp; }
    }

    return fuel;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 07\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = aoc::split_line_by<int>(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>


TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(16,1,2,0,4,2,7,1,2,14)";

    auto input = aoc::split_line_by<int>(ss);

    REQUIRE(37 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(16,1,2,0,4,2,7,1,2,14)";

    auto input = aoc::split_line_by<int>(ss);

    REQUIRE(168 == part2(input));
}

#endif
