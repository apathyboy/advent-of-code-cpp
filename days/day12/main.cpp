#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <range/v3/all.hpp>

#include <cmath>
#include <fstream>
#include <map>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

struct instruction {
    char dir;
    int  amount;
};

std::map<char, glm::ivec2> directions = {
    {'N', glm::ivec2{0, 1}},
    {'E', glm::ivec2{1, 0}},
    {'S', glm::ivec2{0, -1}},
    {'W', glm::ivec2{-1, 0}}};

std::vector<instruction> read_input(std::istream&& input)
{
    // clang-format off
    return ranges::getlines(input) 
        | rv::transform([](auto&& s) { return instruction{s[0], std::stoi(s.substr(1))}; })
        | ranges::to<std::vector>;
    // clang-format on
}

int navigate(const std::vector<instruction>& input, glm::ivec2 heading, bool follow = false)
{
    glm::ivec2 ship_position = {0, 0};

    for (const auto& instr : input) {
        if (instr.dir == 'L' || instr.dir == 'R') {
            rs::for_each(rv::iota(0, instr.amount / 90), [&heading, &instr](auto) {
                std::tie(heading.x, heading.y) = std::make_tuple(
                    heading.y * ((instr.dir == 'L') ? -1 : 1),
                    heading.x * ((instr.dir == 'R') ? -1 : 1));
            });
        }
        else if (instr.dir == 'F') {
            ship_position += heading * instr.amount;
        }
        else {
            (follow ? heading : ship_position) += directions.at(instr.dir) * instr.amount;
        }
    }

    return std::abs(ship_position.x) + std::abs(ship_position.y);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 12\n");

    auto input = read_input(std::ifstream{"days/day12/puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", navigate(input, directions.at('E')));
    fmt::print("Part 2 Solution: {}\n", navigate(input, {10, 1}, true));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can read input to actions")
{
    std::stringstream ss;

    ss << R"(F10
N3
F7
R90
F11)";

    auto input = read_input(std::move(ss));

    REQUIRE(input[0].dir == 'F');
    REQUIRE(input[0].amount == 10);
    REQUIRE(input[3].dir == 'R');
    REQUIRE(input[3].amount == 90);
}


TEST_CASE("Can solve day 12 problems")
{
    std::stringstream ss;

    ss << R"(F10
N3
F7
R90
F11)";

    auto input = read_input(std::move(ss));

    SECTION("Can solve part 1 example") { REQUIRE(25 == navigate(input, directions.at('E'))); }

    SECTION("Can solve part 2 example") { REQUIRE(286 == navigate(input, {10, 1}, true)); }
}

#endif
