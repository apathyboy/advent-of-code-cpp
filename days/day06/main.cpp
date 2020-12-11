
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <set>
#include <string>

namespace ra = ranges::actions;
namespace rv = ranges::views;

int64_t part1(const std::vector<std::string>& input)
{
    // clang-format off
    auto rng = input 
        | rv::split("") 
        | rv::transform([](auto&& rng) { 
            return ranges::distance(rng | rv::join | ranges::to<std::vector> | ra::sort | ra::unique); });
    // clang-format on

    return ranges::accumulate(rng, int64_t{0});
}

int64_t part2(const std::vector<std::string>& input)
{
    // clang-format off
    auto rng = input 
        | rv::split("") 
        | rv::transform([](auto&& rng) {
            auto first = ranges::front(rng)| ranges::to<std::vector>;

            for (auto s : rv::tail(rng)) {
                first = rv::set_intersection(first, s) | ranges::to<std::vector>;
            }

            return ranges::distance(first); });
    // clang-format on

    return ranges::accumulate(rng, int64_t{0});
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 06\n");


    std::ifstream ifs{"days/day06/puzzle.in"};

    auto input = ranges::getlines(ifs) | rv::transform([](auto&& s) {
                     ranges::sort(s);
                     return s;
                 })
                 | ranges::to<std::vector>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
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

    auto input = ranges::getlines(ss) | ranges::to<std::vector>;

    SECTION("Can solve part 1 example") { REQUIRE(11 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(6 == part2(input)); }
}

#endif
