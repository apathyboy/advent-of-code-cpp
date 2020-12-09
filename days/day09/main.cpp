
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>

namespace rv = ranges::views;

std::vector<int64_t> read_input(std::istream&& i)
{
    // clang-format off
    return ranges::getlines(i) 
        | rv::transform([](auto&& s) { return std::stoll(s); })
        | ranges::to<std::vector>;
    // clang-format on
}

int64_t part1(const std::vector<int64_t>& input, int window_size)
{
    // clang-format off
    auto rng = input 
        | rv::sliding(window_size + 1) 
        | rv::filter([](const auto& rng) {
            int64_t target = ranges::front(rng | rv::reverse);

            return ranges::distance(rng 
                | rv::drop_last(1)
                | rv::transform([target](auto i) { return target - i; })
                | rv::unique
                | rv::filter([&rng](const auto& p) { return ranges::contains(rng, p); })) == 0; })
        | rv::transform([](const auto& rng) { return ranges::front(rng | rv::reverse); })
        | rv::take(1);
    // clang-format on

    return ranges::front(rng);
}

int64_t part2(const std::vector<int64_t>& input, int64_t target)
{
    int window_size = 2;

    std::optional<int64_t> result;

    while (window_size < input.size() && !result) {
        auto rng = input | rv::sliding(window_size) | rv::filter([target](const auto& rng) {
                       return ranges::accumulate(rng, int64_t{0}) == target;
                   })
                   | rv::transform([](auto&& rng) {
                         auto [min, max] = ranges::minmax_element(rng);
                         return *min + *max;
                     });

        if (ranges::distance(rng) > 0) {
            result = ranges::front(rng);
        }

        ++window_size;
    }

    return result.value();
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 08\n");

    auto input = read_input(std::ifstream{"days/day09/input.txt"});

    int window_size = 25;

    fmt::print("Part 1 Solution: {}\n", part1(input, window_size));
    fmt::print("Part 2_2 Solution: {}\n", part2(input, part1(input, window_size)));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 9 problems")
{
    std::stringstream ss;

    ss << R"(35
20
15
25
47
40
62
55
65
95
102
117
150
182
127
219
299
277
309
576)";

    auto input       = read_input(std::move(ss));
    auto window_size = 5;

    REQUIRE(20 == input.size());
    REQUIRE(35 == input[0]);
    REQUIRE(309 == input[18]);

    SECTION("Can solve part 1 example") { REQUIRE(127 == part1(input, window_size)); }

    SECTION("Can solve part 2 example")
    {
        REQUIRE(62 == part2(input, part1(input, window_size)));
    }
}

#endif
