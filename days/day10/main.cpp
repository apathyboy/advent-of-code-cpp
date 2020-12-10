#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace ra = ranges::actions;
namespace rv = ranges::views;

int64_t combinations(int64_t d)
{
    // TODO solve for the general case
    if (d == 4) {
        return 7;
    }
    else if (d == 3) {
        return 4;
    }
    else if (d == 2) {
        return 2;
    }

    return 1;
}

auto diff_between_elements(const std::vector<int>& input)
{
    auto highest_rating = ranges::max(input) + 3;

    return rv::zip(
               rv::concat(rv::single(0), input),
               rv::concat(input, rv::single(highest_rating)))
           | rv::transform([](auto&& p) { return p.second - p.first; });
}

int64_t part1(const std::vector<int>& input)
{
    auto diffs = diff_between_elements(input);

    return ranges::count(diffs, 1) * ranges::count(diffs, 3);
}

int64_t part2(const std::vector<int>& input)
{
    // clang-format off
    return ranges::accumulate(
        diff_between_elements(input)
            | rv::group_by(std::equal_to<int>{}) 
            | rv::filter([](const auto& r) { return (ranges::front(r) == 1); })
            | rv::transform([](auto&& r) { return combinations(ranges::distance(r)); }),
        int64_t{1},
        std::multiplies<>{});
    // clang-format on
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 10\n");

    std::ifstream ifs{"days/day10/input.txt"};

    // clang-format off
    auto input = ranges::getlines(ifs) 
        | rv::transform([](auto&& s) { return std::stoi(s); })
        | ranges::to<std::vector> 
        | ra::sort;
    // clang-format on

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>


TEST_CASE("Can solve day 10 problems")
{
    std::stringstream ss;

    ss << R"(16
10
15
5
1
11
7
19
6
12
4)";

    // clang-format off
    auto input = ranges::getlines(ss) 
        | rv::transform([](auto&& s) { return std::stoi(s); })
        | ranges::to<std::vector> 
        | ra::sort;
    // clang-format on

    SECTION("Can solve part 1 example") { REQUIRE(35 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(8 == part2(input)); }
}

#endif
