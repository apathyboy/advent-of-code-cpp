#include <fmt/core.h>

#include <range/v3/all.hpp>

#include <bitset>
#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

int calculate_seat_id(std::string_view pass)
{
    auto to_bits = [](char c) {
        if (c == 'F' || c == 'L') return '0';
        else
            return '1';
    };

    return std::bitset<10>(pass | rv::transform(to_bits) | rs::to<std::string>).to_ulong();
}

int64_t part1(const std::vector<int>& input)
{
    return rs::max(input);
}

int64_t part2(const std::vector<int>& input)
{
    auto [min_id, max_id] = rs::minmax(input);
    auto sum              = rs::accumulate(input, 0);

    return ((input.size() + 1) * (min_id + max_id) / 2) - sum;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 05\n");

    std::ifstream ifs{"puzzle.in"};

    auto input = rs::getlines(ifs) | rv::transform(calculate_seat_id) | rs::to<std::vector<int>>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can calculate seat id from boarding pass", "[day05]")
{
    REQUIRE(357 == calculate_seat_id("FBFBBFFRLR"));
    REQUIRE(567 == calculate_seat_id("BFFFBBFRRR"));
    REQUIRE(119 == calculate_seat_id("FFFBBBFRRR"));
    REQUIRE(820 == calculate_seat_id("BBFFBBFRLL"));
}

#endif
