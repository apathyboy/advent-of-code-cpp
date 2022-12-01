#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

int solve(const std::vector<int>& input, int nth_number)
{
    // clang-format off
    auto cache = input 
        | rv::enumerate 
        | rv::transform([](auto&& p) {
            return std::make_pair(p.second, static_cast<int>(p.first + 1)); })
        | rv::drop_last(1) 
        | rs::to<std::unordered_map<int, int>>;
    // clang-format on

    int last_number = rs::back(input);

    for (int i : rv::iota(static_cast<int>(input.size()), nth_number)) {
        if (cache.contains(last_number)) {
            std::tie(last_number, cache[last_number]) = std::make_tuple(i - cache[last_number], i);
        }
        else {
            cache[last_number] = i;
            last_number        = 0;
        }
    }

    return last_number;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 15\n");

    std::vector<int> input = {1, 0, 18, 10, 19, 6};

    fmt::print("Part 1 Solution: {}\n", solve(input, 2020));
    fmt::print("Part 2 Solution: {}\n", solve(input, 30000000));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve day 15 problems")
{
    SECTION("Can solve example")
    {
        REQUIRE(0 == solve(std::vector{0, 3, 6}, 10));
        REQUIRE(436 == solve(std::vector{0, 3, 6}, 2020));
        REQUIRE(1 == solve(std::vector{1, 3, 2}, 2020));
        REQUIRE(10 == solve(std::vector{2, 1, 3}, 2020));
        REQUIRE(27 == solve(std::vector{1, 2, 3}, 2020));
        REQUIRE(78 == solve(std::vector{2, 3, 1}, 2020));
        REQUIRE(438 == solve(std::vector{3, 2, 1}, 2020));
        REQUIRE(1836 == solve(std::vector{3, 1, 2}, 2020));
    }
}

#endif
