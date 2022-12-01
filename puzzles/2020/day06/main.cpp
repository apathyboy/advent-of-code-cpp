
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <string>

namespace rs = ranges;
namespace ra = ranges::actions;
namespace rv = ranges::views;

std::vector<std::string> parse_input(std::istream&& input)
{
    // clang-format off
    return rs::getlines(input) 
        | rv::transform([](auto && s) { rs::sort(s); return s; })
        | ranges::to<std::vector>;
    // clang-format on
}

int64_t part1(const std::vector<std::string>& input)
{
    // clang-format off
    auto rng = input 
        | rv::split("") 
        | rv::transform([](auto&& rng) { 
            return rs::distance(rng | rv::join | rs::to<std::vector> | ra::sort | ra::unique); });
    // clang-format on

    return rs::accumulate(rng, int64_t{0});
}

int64_t part2(const std::vector<std::string>& input)
{
    // clang-format off
    auto rng = input 
        | rv::split("") 
        | rv::transform([](auto&& rng) {
            auto first = rs::front(rng) | rs::to<std::vector>;

            for (auto s : rv::tail(rng)) {
                first = rv::set_intersection(first, s) | rs::to<std::vector>;
            }

            return rs::distance(first); });
    // clang-format on

    return rs::accumulate(rng, int64_t{0});
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 06\n");

    auto input = parse_input(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve day 6 problems")
{
    std::stringstream ss;

    ss << R"(abc

a
b
c

ab
ac

a
a
a
a

b)";

    auto input = parse_input(std::move(ss));

    SECTION("Can solve part 1 example") { REQUIRE(11 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(6 == part2(input)); }
}

#endif
