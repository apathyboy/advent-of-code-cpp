#include <fmt/core.h>

#pragma warning(push)
#pragma warning(disable : 28278)
#include <range/v3/all.hpp>
#pragma warning(pop)

#include <bitset>
#include <fstream>

namespace rv = ranges::views;

int calculate_seat_id(std::string_view pass)
{
    auto to_bits = [](char c) {
        if (c == 'F' || c == 'L')
            return '0';
        else
            return '1';
    };

    return std::bitset<10>(pass | rv::transform(to_bits) | ranges::to<std::string>).to_ulong();
}

int64_t part1(const std::vector<int>& input)
{
    return ranges::max(input);
}

int64_t part2(const std::vector<int>& input)
{
    auto [min_id, max_id] = ranges::minmax(input);
    auto sum              = ranges::accumulate(input, 0);

    return ((input.size() + 1) * (min_id + max_id) / 2) - sum;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 05\n");

    std::ifstream ifs{"days/day05/input.txt"};

    auto input = ranges::getlines(ifs) | rv::transform(calculate_seat_id)
                 | ranges::to<std::vector<int>>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can calculate seat id from boarding pass", "[day05]")
{
    REQUIRE(357 == calculate_seat_id("FBFBBFFRLR"));
    REQUIRE(567 == calculate_seat_id("BFFFBBFRRR"));
    REQUIRE(119 == calculate_seat_id("FFFBBBFRRR"));
    REQUIRE(820 == calculate_seat_id("BBFFBBFRLL"));
}

#endif
