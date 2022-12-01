#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <algorithm>
#include <fstream>
#include <ranges>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct Image {
    std::vector<bool>              decoder;
    std::vector<std::vector<bool>> data;
    bool                           default_value = false;

    uint32_t encoded_number(int64_t line, int64_t column) const;
    void     enhance();
    uint64_t lit_pixels() const;

    friend std::istream& operator>>(std::istream&, Image&);
    friend std::ostream& operator<<(std::ostream&, const Image&);
};


uint32_t Image::encoded_number(int64_t line, int64_t column) const
{
    uint32_t result = 0;
    for (int64_t i = line - 1; i <= line + 1; i++)
        for (int64_t j = column - 1; j <= column + 1; j++)
            if (i >= 0 && i < std::ranges::ssize(data) && j >= 0 && j < std::ranges::ssize(data[i]))
                result = (result << 1) | data[i][j];
            else
                result = (result << 1) | default_value;

    return result;
}

void Image::enhance()
{
    std::vector<std::vector<bool>> new_data(data.size() + 2, std::vector<bool>(data.size() + 2, false));
    for (int64_t i = -1; i <= std::ranges::ssize(data); i++)
        for (int64_t j = -1; j <= std::ranges::ssize(data[0]); j++)
            new_data[i + 1][j + 1] = decoder[encoded_number(i, j)];

    default_value = default_value ? decoder[0b111111111] : decoder[0];
    std::swap(data, new_data);
}

uint64_t Image::lit_pixels() const
{
    uint64_t lit = 0;
    if (default_value) return std::numeric_limits<uint32_t>::max();

    for (auto& line : data)
        lit += rs::count(line, true);
    return lit;
}


std::istream& operator>>(std::istream& s, Image& image)
{
    s >> std::noskipws;
    auto line_view = std::ranges::istream_view<char>(s) | std::views::drop_while(isspace)
                     | std::ranges::views::take_while([](char c) { return c != '\n'; })
                     | std::ranges::views::transform([](char c) { return c == '#'; });
    std::ranges::copy(line_view, std::back_insert_iterator(image.decoder));
    while (!s.eof()) {
        std::vector<bool> line;
        std::ranges::copy(line_view, std::back_insert_iterator(line));
        image.data.emplace_back(std::move(line));
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, const Image& image)
{
    auto visual = std::views::transform([](bool b) { return b ? '#' : '.'; });
    std::ranges::copy(image.decoder | visual, std::ostream_iterator<char>(s));
    s << "\n\n";
    for (auto& line : image.data) {
        std::ranges::copy(line | visual, std::ostream_iterator<char>(s));
        s << "\n";
    }
    return s;
}

uint64_t part1(Image input)
{
    input.enhance();
    input.enhance();
    return input.lit_pixels();
}

uint64_t part2(Image input)
{
    for (int i = 0; i < 50; i++)
        input.enhance();
    return input.lit_pixels();
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 20\n");

    std::ifstream ifs{"puzzle.in"};

    Image input;
    ifs >> input;

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
    ss << R"(..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###)";

    Image input;
    ss >> input;

    REQUIRE(35 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..###..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###.######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#..#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#......#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.....####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#.......##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#

#..#.
#....
##..#
..#..
..###)";

    Image input;
    ss >> input;

    REQUIRE(3351 == part2(input));
}

#endif