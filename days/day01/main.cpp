#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <array>
#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

auto contains_last_element(const std::vector<int>& input)
{
    return [&input](const auto& p) { return rs::contains(input, p.back()); };
}

auto accumulate_multiply()
{
    return [](auto&& p) { return rs::accumulate(p, 1, std::multiplies<>()); };
}

int part1(const std::vector<int>& input)
{
    // clang-format off
    auto result = input 
        | rv::transform([](auto i) { return std::array{i, 2020 - i}; })
        | rv::filter(contains_last_element(input))
        | rv::transform(accumulate_multiply());
    // clang-format on

    return rs::front(result);
}

int part2(const std::vector<int>& input)
{
    // clang-format off
    auto result = rv::cartesian_product(input, input)
        | rv::transform([](auto&& i) { 
            auto [a, b] = i;
            return std::array{a, b, 2020 - a - b}; })
        | rv::filter(contains_last_element(input))
        | rv::transform(accumulate_multiply());
    // clang-format on

    return rs::front(result);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 01\n");

    auto input = aoc::read_int_per_line(std::ifstream{"days/day01/puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 2 problems")
{
    std::stringstream ss;

    ss << R"(1721
979
366
299
675
1456)";

    // clang-format off
    auto input = aoc::read_int_per_line(std::move(ss));
    // clang-format on

    SECTION("Can solve part 1 example") { REQUIRE(514579 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(241861950 == part2(input)); }
}

#endif
