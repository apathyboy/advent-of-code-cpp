#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <glm/vec2.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <range/v3/all.hpp>

#include <fstream>
#include <unordered_set>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct puzzle_t {
    std::unordered_set<glm::ivec2> dots;
    std::vector<glm::ivec2>        folds;
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
                glm::ivec2 dot{};
                dot.x = std::stoi(line.substr(0, line.find_first_of(',')));
                dot.y = std::stoi(line.substr(line.find_first_of(',') + 1));

                p.dots.insert(dot);
            }
            else {
                glm::ivec2 fold{};
                int        val = std::stoi(line.substr(13));

                if (line[11] == 'x') { fold.x = val; }
                else {
                    fold.y = val;
                }

                p.folds.push_back(fold);
            }
        }
    }

    return p;
}

bool contains(const std::vector<glm::ivec2>& c, const glm::ivec2& s)
{
    for (const auto& t : c) {
        if (t.x == s.x && t.y == s.y) return true;
    }

    return false;
}

void fold_puzzle(puzzle_t& input, const glm::ivec2& fold)
{
    std::unordered_set<glm::ivec2> keep_dots;

    for (auto dot : input.dots) {
        if (fold.x > 0) {
            if (dot.x <= fold.x) { keep_dots.insert(dot); }
            else {
                dot.x = (fold.x * 2) - dot.x;
                keep_dots.insert(dot);
            }
        }
        else if (fold.y > 0) {
            if (dot.y <= fold.y) { keep_dots.insert(dot); }
            else {
                dot.y = (fold.y * 2) - dot.y;
                keep_dots.insert(dot);
            }
        }
    }

    input.dots = std::move(keep_dots);
}

int64_t part1(puzzle_t input)
{
    auto fold = input.folds[0];

    fold_puzzle(input, fold);

    return input.dots.size();
}

std::vector<std::string> part2(puzzle_t input)
{
    for (auto fold : input.folds) {
        fold_puzzle(input, fold);
    }

    // clang-format off
    auto max_x = rs::min(input.folds 
        | rv::filter([](const auto& f) { return f.x > 0; })
        | rv::transform([](const auto& f) { return f.x; }));

    auto max_y = rs::min(input.folds 
        | rv::filter([](const auto& f) { return f.y > 0; })
        | rv::transform([](const auto& f) { return f.y; }));
    // clang-format on

    std::vector<std::string> display(max_y, std::string(max_x, '.'));

    for (const auto& p : input.dots) {
        display[p.y][p.x] = '#';
    }

    return display;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 13\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution\n\n");

    for (const auto& l : part2(input)) {
        fmt::print("{}\n", l);
    }
}

#else

#include <catch2/catch_test_macros.hpp>
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

    auto input = read_input(ss);

    REQUIRE(17 == part1(input));
}

TEST_CASE("Can solve part 2 example")
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

    auto input = read_input(ss);

    // clang-format off
    std::vector<std::string> output = {
        "#####",
        "#...#",
        "#...#",
        "#...#",
        "#####",
        ".....",
        "....."};
    // clang-format on

    REQUIRE(output == part2(input));
}

#endif
