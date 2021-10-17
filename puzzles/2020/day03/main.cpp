
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <string>

namespace rs = ranges;
namespace rv = ranges::views;

auto slope_type1_hits(const std::vector<std::string>& input, int slope)
{
    auto hit_test = [slope](auto&& p) {
        auto&& [depth, line] = p;
        return (line[(depth * slope) % line.length()] == '#') ? 1 : 0;
    };

    return rs::accumulate(rv::enumerate(input) | rv::transform(hit_test), 0);
}

auto slope_type2_hits(const std::vector<std::string>& input)
{
    auto hit_test = [](auto&& p) {
        auto&& [depth, line] = p;
        return (depth % 2 == 0 && line[(depth / 2) % line.length()] == '#') ? 1 : 0;
    };

    return rs::accumulate(rv::enumerate(input) | rv::transform(hit_test), 0);
}

int part1(const std::vector<std::string>& input)
{
    return slope_type1_hits(input, 3);
}

int part2(const std::vector<std::string>& input)
{
    return slope_type1_hits(input, 1) * slope_type1_hits(input, 3) * slope_type1_hits(input, 5)
           * slope_type1_hits(input, 7) * slope_type2_hits(input);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 03\n");

    std::ifstream ifs{"puzzle.in"};

    auto input = rs::getlines(ifs) | rs::to<std::vector>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 3 problems")
{
    std::stringstream ss;

    ss << R"(..##.......
#...#...#..
.#....#..#.
..#.#...#.#
.#...##..#.
..#.##.....
.#.#.#....#
.#........#
#.##...#...
#...##....#
.#..#...#.#)";

    auto input = rs::getlines(ss) | rs::to<std::vector>;

    SECTION("Can solve part 1 example") { REQUIRE(7 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(336 == part2(input)); }
}

#endif
