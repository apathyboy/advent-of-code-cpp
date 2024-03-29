
#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <numeric>
#include <string>
#include <vector>
#include <iostream>

namespace rs = ranges;
namespace rv = ranges::views;

int part1(std::istream&& input)
{
    glm::ivec2 pos{0, 0};

    for (auto str : rs::getlines(input)) {
        auto dir = str[0];
        int  amt = std::stoi(str.substr(str.find(' ') + 1));

        if (dir == 'u') pos.y -= amt;
        else if (dir == 'd')
            pos.y += amt;
        else if (dir == 'f')
            pos.x += amt;
    }

    return pos.x * pos.y;
}


int part2(std::istream&& input)
{
    int        aim = 0;
    glm::ivec2 pos{0, 0};

    for (auto str : rs::getlines(input)) {
        auto dir = str[0];
        int  amt = std::stoi(str.substr(str.find(' ') + 1));

        if (dir == 'u') aim -= amt;
        else if (dir == 'd')
            aim += amt;
        else if (dir == 'f')
            pos += glm::ivec2{amt, amt * aim};
    }

    return pos.x * pos.y;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 02\n");

    std::string input_path = "puzzle.in";

    fmt::print("Part 1 Solution: {}\n", part1(std::ifstream{input_path}));
    fmt::print("Part 2 Solution: {}\n", part2(std::ifstream{input_path}));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";

    REQUIRE(150 == part1(std::move(ss)));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";

    REQUIRE(900 == part2(std::move(ss)));
}

#endif
