#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>

#include <iostream>

namespace rs = ranges;
namespace rv = ranges::views;

struct tile {
    int                            id;
    std::vector<std::vector<char>> image_data;
};

int parse_tile_id(const std::string& s)
{
    return std::stoi(s.substr(5, s.length() - 1));
}

std::vector<tile> read_input(std::istream&& input)
{
    return rs::getlines(input) | rv::split("") | rv::transform([](auto&& rng) {
               auto v = rng | rv::transform([](auto&& s) { return s | rs::to<std::string>; })
                        | rs::to_vector;

               tile t{parse_tile_id(v[0]), rv::tail(v) | rs::to<std::vector<std::vector<char>>>};

               return t;
           })
           | rs::to_vector;
}

int64_t part1()
{
    return 0;
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 20\n");

    std::string input_path = "days/day20/puzzle.in";

    auto input = read_input(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1());
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"()";

    REQUIRE(0 == part1());
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
