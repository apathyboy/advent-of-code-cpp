#include <aoc/aoc.hpp>

#include <glm/vec2.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <deque>
#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

using cave_t = std::vector<std::vector<uint8_t>>;

cave_t read_input(std::istream& input)
{
    return rs::getlines(input) | rv::transform([](auto&& s) {
               return s | rv::transform([](char c) { return static_cast<uint8_t>(c - '0'); })
                      | rs::to<std::vector>;
           })
           | rs::to<std::vector>;
}

struct path_t {
    std::vector<glm::ivec2> steps;
    int                     min_risk = 0;
};

size_t least_path_cost(const cave_t& cave)
{
    std::vector<std::vector<size_t>> cost(
        cave.size(),
        std::vector<size_t>(cave.size(), std::numeric_limits<size_t>::max()));
    cost[0][0] = 0;

    std::deque<std::pair<size_t, size_t>> pending;
    pending.push_back({0, 0});

    while (!pending.empty()) {
        auto [row, col] = pending.front();
        pending.pop_front();

        if (row > 0 && cost[row][col] + cave[row - 1][col] < cost[row - 1][col]) {
            cost[row - 1][col] = cost[row][col] + cave[row - 1][col];
            pending.push_back({row - 1, col});
        }
        if (row + 1 < cost.size() && cost[row][col] + cave[row + 1][col] < cost[row + 1][col]) {
            cost[row + 1][col] = cost[row][col] + cave[row + 1][col];
            pending.push_back({row + 1, col});
        }
        if (col > 0 && cost[row][col] + cave[row][col - 1] < cost[row][col - 1]) {
            cost[row][col - 1] = cost[row][col] + cave[row][col - 1];
            pending.push_back({row, col - 1});
        }
        if (col + 1 < cost[row].size() && cost[row][col] + cave[row][col + 1] < cost[row][col + 1]) {
            cost[row][col + 1] = cost[row][col] + cave[row][col + 1];
            pending.push_back({row, col + 1});
        }
    }

    return cost[cost.size() - 1][cost[cost.size() - 1].size() - 1];
}

cave_t expand_cave(const cave_t& cave)
{
    const size_t dim = cave.size();
    cave_t       result(dim * 5, std::vector<uint8_t>(dim * 5, 0));
    for (size_t i = 0; i < result.size(); i++) {
        for (size_t j = 0; j < result[i].size(); j++) {
            result[i][j] = (((cave[i % dim][j % dim] + i / dim + j / dim) - 1) % 9) + 1;
        }
    }
    return result;
}

size_t part1(const cave_t& input)
{
    return least_path_cost(input);
}

size_t part2(const cave_t& input)
{
    cave_t big_cave = expand_cave(input);
    return least_path_cost(big_cave);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 15\n");

    std::ifstream ifs{"puzzle.in"};

    auto input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581)";

    auto input = read_input(ss);

    REQUIRE(40 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(1163751742
1381373672
2136511328
3694931569
7463417111
1319128137
1359912421
3125421639
1293138521
2311944581)";

    auto input = read_input(ss);

    REQUIRE(315 == part2(input));
}

#endif
