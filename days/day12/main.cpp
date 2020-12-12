#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <range/v3/all.hpp>

#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

namespace ra = ranges::actions;
namespace rv = ranges::views;

struct action {
    char type;
    int  amount;
};

std::vector<action> read_input(std::istream&& input)
{
    // clang-format off
    return ranges::getlines(input) 
        | rv::transform([](auto&& s) { return action{s[0], std::stoi(s.substr(1))}; })
        | ranges::to<std::vector>;
    // clang-format on
}

char turn(action step, char current_heading)
{
    auto dirs = std::array{'N', 'W', 'S', 'E'};

    auto start      = ranges::distance(dirs.begin(), ranges::find(dirs, current_heading));
    auto turn_steps = (step.amount / 90) * (step.type == 'L' ? 1 : -1);

    return dirs[(start + turn_steps) % dirs.size()];
}

glm::ivec2 move_step(char heading, int amount)
{
    glm::ivec2 move = {0, 0};

    switch (heading) {
        case 'N': {
            move += glm::ivec2{0, amount};
        } break;
        case 'S': {
            move += glm::ivec2{0, -amount};
        } break;
        case 'E': {
            move += glm::ivec2{amount, 0};
        } break;
        case 'W': {
            move += glm::ivec2{-amount, 0};
        } break;
    }

    return move;
}

glm::ivec2 move_around(action step, glm::ivec2 current_pos)
{
    for (int i = 0; i < step.amount / 90; ++i) {
        std::swap(current_pos.x, current_pos.y);
        current_pos *= (step.type == 'L') ? glm::ivec2{-1, 1} : glm::ivec2{1, -1};
    }

    return current_pos;
}

int64_t part1(const std::vector<action>& input)
{
    char current_heading = 'E';

    glm::ivec2 pos = {0, 0};

    for (const auto& step : input) {
        switch (step.type) {
            case 'N':
            case 'S':
            case 'E':
            case 'W': {
                pos += move_step(step.type, step.amount);
            } break;
            case 'L':
            case 'R': {
                current_heading = turn(step, current_heading);
            } break;
            case 'F': {
                pos += move_step(current_heading, step.amount);
            } break;
        }
    }

    return std::abs(pos.x) + std::abs(pos.y);
}

int64_t part2(const std::vector<action>& input)
{
    glm::ivec2 waypoint_pos = {10, 1};
    glm::ivec2 ship_pos     = {0, 0};

    for (const auto& step : input) {
        switch (step.type) {
            case 'N':
            case 'S':
            case 'E':
            case 'W': {
                waypoint_pos += move_step(step.type, step.amount);
            } break;
            case 'L':
            case 'R': {
                waypoint_pos = move_around(step, waypoint_pos);
            } break;
            case 'F': {
                ship_pos += waypoint_pos * step.amount;
            } break;
        }
    }

    return std::abs(ship_pos.x) + std::abs(ship_pos.y);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 12\n");

    auto input = read_input(std::ifstream{"days/day12/puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

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

    REQUIRE(input[0].type == 'F');
    REQUIRE(input[0].amount == 10);
    REQUIRE(input[3].type == 'R');
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

    SECTION("Can solve part 1 example") { REQUIRE(25 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(286 == part2(input)); }
}

#endif
