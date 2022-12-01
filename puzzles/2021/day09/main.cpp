#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

std::vector<std::vector<int>> read_input(std::istream& input)
{
    return rs::getlines(input) | rv::transform([](auto&& s) {
               return s | rv::transform([](char c) { return c - '0'; }) | rs::to<std::vector>;
           })
           | rs::to<std::vector>;
}

int part1(const std::vector<std::vector<int>>& input)
{
    int risk_levels_sum = 0;

    for (int y = 0; y < input.size(); ++y) {
        for (int x = 0; x < input[y].size(); ++x) {
            int i = input[y][x];

            bool low = true;

            // check up
            if (y > 0) { low = low && i < input[y - 1][x]; }
            // check down
            if (y < input.size() - 1) { low = low && i < input[y + 1][x]; }
            // check left
            if (x > 0) { low = low && i < input[y][x - 1]; }
            // check right
            if (x < input[y].size() - 1) { low = low && i < input[y][x + 1]; }


            if (low) risk_levels_sum += i + 1;
        }
    }

    return risk_levels_sum;
}

bool contains(const std::vector<std::pair<int, int>>& explored, int x, int y)
{
    auto it = rs::find(explored, std::pair{x, y});
    return it != explored.end();
}

int explore_basin(
    const std::vector<std::vector<int>>& input,
    int                                  x,
    int                                  y,
    std::vector<std::pair<int, int>>&    explored)
{
    if (contains(explored, x, y) || input[y][x] == 9) return 0;

    explored.push_back({x, y});
    int points_in_basin = 1;

    // check up
    if (y > 0) { points_in_basin += explore_basin(input, x, y - 1, explored); }
    // check down
    if (y < input.size() - 1) { points_in_basin += explore_basin(input, x, y + 1, explored); }
    // check left
    if (x > 0) { points_in_basin += explore_basin(input, x - 1, y, explored); }
    // check right
    if (x < input[y].size() - 1) { points_in_basin += explore_basin(input, x + 1, y, explored); }

    return points_in_basin;
}

int part2(const std::vector<std::vector<int>>& input)
{
    std::vector<int> basins;

    for (int y = 0; y < input.size(); ++y) {
        for (int x = 0; x < input[y].size(); ++x) {
            int i = input[y][x];

            bool low = true;

            // check up
            if (y > 0) { low = low && i < input[y - 1][x]; }
            // check down
            if (y < input.size() - 1) { low = low && i < input[y + 1][x]; }
            // check left
            if (x > 0) { low = low && i < input[y][x - 1]; }
            // check right
            if (x < input[y].size() - 1) { low = low && i < input[y][x + 1]; }


            if (low) {
                std::vector<std::pair<int, int>> visited;
                basins.push_back(explore_basin(input, x, y, visited));
            }
        }
    }

    rs::sort(basins);

    return rs::accumulate(basins | rv::reverse | rv::take(3), 1, std::multiplies<>());
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 09\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(2199943210
3987894921
9856789892
8767896789
9899965678)";

    auto input = read_input(ss);

    REQUIRE(15 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(2199943210
3987894921
9856789892
8767896789
9899965678)";

    auto input = read_input(ss);

    REQUIRE(1134 == part2(input));
}

#endif
