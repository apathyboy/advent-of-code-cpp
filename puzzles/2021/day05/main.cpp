#include <aoc/aoc.hpp>

#include <fmt/core.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <unordered_map>
#include <regex>
#include <string>

namespace rs = ranges;
namespace rv = ranges::views;

struct line_t {
    glm::ivec2 a;
    glm::ivec2 b;
};

line_t parse_line(const std::string& str)
{
    auto static const rx = std::regex{"(\\d+),(\\d+) -> (\\d+),(\\d+)"};

    std::smatch m;
    std::regex_search(str, m, rx);

    return {{std::stoi(m[1]), std::stoi(m[2])}, {std::stoi(m[3]), std::stoi(m[4])}};
}

std::vector<line_t> read_input(std::istream&& input)
{
    return rs::getlines(input) | rv::transform(parse_line) | rs::to<std::vector>;
}

bool is_horizontal(const line_t& line)
{
    return line.a.y == line.b.y;
}

bool is_vertical(const line_t& line)
{
    return line.a.x == line.b.x;
}

bool is_horizontal_or_vertical(const line_t& line)
{
    return is_horizontal(line) || is_vertical(line);
}

int count_overlaps(const std::vector<line_t>& lines)
{
    std::unordered_map<glm::ivec2, int> plots;

    int counter = 0;

    for (const auto& line : lines) {
        int x_step = is_vertical(line) ? 0 : (line.a.x > line.b.x) ? -1 : 1;
        int y_step = is_horizontal(line) ? 0 : (line.a.y > line.b.y) ? -1 : 1;

        glm::ivec2 tmp = line.a;
        if (++plots[tmp] == 2) { ++counter; }

        while (tmp != line.b) {
            tmp += glm::ivec2{x_step, y_step};
            if (++plots[tmp] == 2) { ++counter; }
        }
    }

    return counter;
}

int part1(const std::vector<line_t>& lines)
{
    return count_overlaps(lines | rv::filter(is_horizontal_or_vertical) | rs::to<std::vector>);
}

int part2(const std::vector<line_t>& lines)
{
    return count_overlaps(lines);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 05\n");

    auto input = read_input(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2)";

    auto lines = read_input(std::move(ss));

    REQUIRE(5 == part1(lines));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(0,9 -> 5,9
8,0 -> 0,8
9,4 -> 3,4
2,2 -> 2,1
7,0 -> 7,4
6,4 -> 2,0
0,9 -> 2,9
3,4 -> 1,4
0,0 -> 8,8
5,5 -> 8,2)";

    auto lines = read_input(std::move(ss));

    REQUIRE(12 == part2(lines));
}

#endif
