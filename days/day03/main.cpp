
#include <fmt/core.h>

#include <fstream>
#include <string>

int part1(std::istream&& input)
{
    std::string str;
    int         slope = 3, depth = 0, count = 0;

    while (std::getline(input, str)) {
        if (str[(depth * slope) % str.length()] == '#')
            ++count;

        ++depth;
    }

    return count;
}

int part2(std::istream&& input)
{
    std::string str;
    int         slope1 = 1, slope2 = 3, slope3 = 5, slope4 = 7;
    int         count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;
    int         depth = 0;

    while (std::getline(input, str)) {
        if (str[(depth * slope1) % str.length()] == '#')
            ++count1;

        if (str[(depth * slope2) % str.length()] == '#')
            ++count2;

        if (str[(depth * slope3) % str.length()] == '#')
            ++count3;

        if (str[(depth * slope4) % str.length()] == '#')
            ++count4;

        if (depth % 2 == 0 && str[(depth / 2) % str.length()] == '#')
            ++count5;

        ++depth;
    }

    return count1 * count2 * count3 * count4 * count5;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 03\n");

    fmt::print("Part 1 Solution: {}\n", part1(std::ifstream{"days/day03/input.txt"}));
    fmt::print("Part 2 Solution: {}\n", part2(std::ifstream{"days/day03/input.txt"}));

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

    SECTION("Can solve part 1 example") { REQUIRE(7 == part1(std::move(ss))); }

    SECTION("Can solve part 2 example") { REQUIRE(336 == part2(std::move(ss))); }
}

#endif
