#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

using map_t = std::vector<std::vector<char>>;

map_t read_input(std::istream& input)
{
    return rs::getlines(input) | rv::transform([](auto&& r) { return r | rs::to<std::vector>; })
           | rs::to<std::vector>;
}

bool has_moves(const std::vector<std::vector<bool>>& potentials)
{
    for (const auto& p : potentials) {
        if (rs::any_of(p, [](bool v) { return v; })) { return true; }
    }
    return false;
}

int part1(map_t input)
{
    int steps = 0;

    std::vector<std::vector<bool>> east_moves(input.size(), std::vector<bool>(input[0].size(), false));
    std::vector<std::vector<bool>> south_moves(input.size(), std::vector<bool>(input[0].size(), false));

    do {
        ++steps;

        east_moves = std::vector<std::vector<bool>>(
            input.size(),
            std::vector<bool>(input[0].size(), false));
        south_moves = std::vector<std::vector<bool>>(
            input.size(),
            std::vector<bool>(input[0].size(), false));

        // check east moves
        for (int y = 0; y < input.size(); ++y) {
            for (int x = 0; x < input[0].size(); ++x) {
                if (input[y][x] != '>') continue;
                if (x == input[0].size() - 1) { east_moves[y][x] = input[y][0] == '.'; }
                else {
                    east_moves[y][x] = input[y][x + 1] == '.';
                }
            }
        }

        // move east
        for (int y = 0; y < input.size(); ++y) {
            for (int x = 0; x < input[0].size(); ++x) {
                if (east_moves[y][x]) {
                    input[y][x] = '.';

                    if (x == input[0].size() - 1) { input[y][0] = '>'; }
                    else {
                        input[y][x + 1] = '>';
                    }
                }
            }
        }

        // check south moves
        for (int y = 0; y < input.size(); ++y) {
            for (int x = 0; x < input[0].size(); ++x) {
                if (input[y][x] != 'v') continue;
                if (y == input.size() - 1) { south_moves[y][x] = input[0][x] == '.'; }
                else {
                    south_moves[y][x] = input[y + 1][x] == '.';
                }
            }
        }

        // move south
        for (int y = 0; y < input.size(); ++y) {
            for (int x = 0; x < input[0].size(); ++x) {
                if (south_moves[y][x]) {
                    input[y][x] = '.';

                    if (y == input.size() - 1) { input[0][x] = 'v'; }
                    else {
                        input[y + 1][x] = 'v';
                    }
                }
            }
        }


    } while (has_moves(east_moves) || has_moves(south_moves));

    return steps;
}

int part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 25\n");

    std::ifstream ifs{"puzzle.in"};

    auto input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2());
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(v...>>.vv>
.vv>>.vv..
>>.>v>...v
>>v>>.>.v.
v>v.vv.v..
>.>>..v...
.vv..>.>v.
v.v..>>v.v
....v..v.>)";

    auto input = read_input(ss);

    REQUIRE(58 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
