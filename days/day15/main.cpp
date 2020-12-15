#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;


int64_t solve(const std::vector<int>& input, int nth_number)
{
    std::unordered_map<int, std::vector<int>> cache;

    int last_number  = 0;
    int current_turn = 1;

    for (auto i : input) {
        last_number = i;
        cache[last_number].push_back(current_turn++);
    }

    while (current_turn <= nth_number) {
        if (cache[last_number].size() == 1) { last_number = 0; }
        else {
            auto rng    = cache[last_number] | rv::reverse | rv::take(2);
            last_number = rng[0] - rng[1];
        }

        cache[last_number].push_back(current_turn++);
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

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
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
