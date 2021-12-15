#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct puzzle_t {
    std::vector<glm::ivec2> dots;
    std::vector<glm::ivec2> folds;
};

puzzle_t read_input(std::istream& input)
{
    puzzle_t p{};

    auto lines = rs::getlines(input) | rs::to<std::vector>;
    bool folds = false;
    for (int i = 0; i < lines.size(); ++i) {
        const auto& line = lines[i];

        if (line == "") { folds = true; }
        else {
            if (!folds) {
                glm::ivec2 dot;
                dot.x = std::stoi(line.substr(0, line.find_first_of(',')));
                dot.y = std::stoi(line.substr(line.find_first_of(',') + 1));

                p.dots.push_back(dot);
            }
            else {
                glm::ivec2 fold;
                int        val = std::stoi(line.substr(13));

                if (line[12] == 'x') { fold.x = val; }
                else {
                    fold.y = val;
                }

                p.folds.push_back(fold);
            }
        }
    }

    return p;
}

int part1()
{
    return 0;
}

int part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 13\n");

    std::ifstream ifs{"puzzle.in"};

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(6,10
0,14
9,10
0,3
10,4
4,11
6,0
6,12
4,1
0,13
10,12
3,4
3,0
8,4
1,10
2,14
8,10
9,0

fold along y=7
fold along x=5)";

    REQUIRE(0 == part1());
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"()";

    REQUIRE(0 == part2());
}

#endif