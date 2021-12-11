#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

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

bool contains(const std::vector<std::pair<int, int>>& explored, int x, int y)
{
    auto it = rs::find(explored, std::pair{x, y});
    return it != explored.end();
}

void explore(
    std::vector<std::vector<int>>&    input,
    int                               x,
    int                               y,
    std::vector<std::pair<int, int>>& flashed)
{
    if (contains(flashed, x, y)) return;

    flashed.push_back({x, y});

    // check up
    if (y > 0) {
        input[y - 1][x]++;
        if (input[y - 1][x] > 9) { explore(input, x, y - 1, flashed); }
    }
    // check up-left
    if (y > 0 && x > 0) {
        input[y - 1][x - 1]++;
        if (input[y - 1][x - 1] > 9) { explore(input, x - 1, y - 1, flashed); }
    }
    // check left
    if (x > 0) {
        input[y][x - 1]++;
        if (input[y][x - 1] > 9) { explore(input, x - 1, y, flashed); }
    }
    // check down-left
    if (y < input.size() - 1 && x > 0) {
        input[y + 1][x - 1]++;
        if (input[y + 1][x - 1] > 9) { explore(input, x - 1, y + 1, flashed); }
    }
    // check down
    if (y < input.size() - 1) {
        input[y + 1][x]++;
        if (input[y + 1][x] > 9) { explore(input, x, y + 1, flashed); }
    }
    // check down-right
    if (y < input.size() - 1 && x < input[y].size() - 1) {
        input[y + 1][x + 1]++;
        if (input[y + 1][x + 1] > 9) { explore(input, x + 1, y + 1, flashed); }
    }
    // check right
    if (x < input[y].size() - 1) {
        input[y][x + 1]++;
        if (input[y][x + 1] > 9) { explore(input, x + 1, y, flashed); }
    }
    // check up-right
    if (y > 0 && x < input[y].size() - 1) {
        input[y - 1][x + 1]++;
        if (input[y - 1][x + 1] > 9) { explore(input, x + 1, y - 1, flashed); }
    }
}

int part1(std::vector<std::vector<int>> input)
{
    int flashes = 0;

    for (int i = 0; i < 100; ++i) {
        for (auto& row : input) {
            for (auto& pos : row) {
                ++pos;
            }
        }

        std::vector<std::pair<int, int>> flashed;

        for (int y = 0; y < input.size(); ++y) {
            for (int x = 0; x < input[y].size(); ++x) {
                if (input[y][x] > 9) { explore(input, x, y, flashed); }
            }
        }

        for (auto& row : input) {
            for (auto& pos : row) {
                if (pos > 9) {
                    ++flashes;
                    pos = 0;
                }
            }
        }
    }

    return flashes;
}

int part2(std::vector<std::vector<int>> input)
{
    int flashes = 0;
    int turn    = 0;

    size_t num_octs = input.size() * input[0].size();

    while (flashes != num_octs) {
        flashes = 0;

        for (auto& row : input) {
            for (auto& pos : row) {
                ++pos;
            }
        }

        std::vector<std::pair<int, int>> flashed;

        for (int y = 0; y < input.size(); ++y) {
            for (int x = 0; x < input[y].size(); ++x) {
                if (input[y][x] > 9) { explore(input, x, y, flashed); }
            }
        }

        for (auto& row : input) {
            for (auto& pos : row) {
                if (pos > 9) {
                    ++flashes;
                    pos = 0;
                }
            }
        }

        ++turn;
    }

    return turn;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 11\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(ifs);


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
    ss << R"(5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526)";

    auto input = read_input(ss);

    REQUIRE(1656 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(5483143223
2745854711
5264556173
6141336146
6357385478
4167524645
2176841721
6882881134
4846848554
5283751526)";

    auto input = read_input(ss);

    REQUIRE(195 == part2(input));
}

#endif