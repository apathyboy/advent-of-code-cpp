
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <cstdint>
#include <fstream>
#include <optional>

namespace rs = ranges;
namespace rv = ranges::views;

std::vector<int64_t> read_input(std::istream&& i)
{
    // clang-format off
    return rs::getlines(i) 
        | rv::transform([](auto&& s) ->int64_t { return std::stoll(s); })
        | rs::to<std::vector>;
    // clang-format on
}

int64_t part1(const std::vector<int64_t>& input, int window_size)
{
    // clang-format off
    auto rng = input 
        | rv::sliding(window_size + 1) 
        | rv::filter([](const auto& rng) {
            int64_t target = rs::back(rng);

            return rs::distance(rng 
                | rv::drop_last(1)
                | rv::transform([target](auto i) { return target - i; })
                | rv::unique
                | rv::filter([&rng](const auto& p) { return rs::contains(rng, p); })) == 0; })
        | rv::transform([](const auto& rng) { return rs::back(rng); })
        | rv::take(1);
    // clang-format on

    return rs::front(rng);
}

int64_t part2(const std::vector<int64_t>& input, int64_t target)
{
    std::optional<int64_t> result;

    for (uint32_t window_size = 2; window_size < input.size() && !result;) {
        // clang-format off
        auto rng = input 
            | rv::sliding(window_size++) 
            | rv::filter([target](const auto& rng) { return rs::accumulate(rng, int64_t{0}) == target; })
            | rv::transform([](auto&& rng) {
                auto [min, max] = rs::minmax_element(rng);
                return *min + *max; });
        // clang-format on

        if (rs::distance(rng) > 0) { result = rs::front(rng); }
    }

    return result.value();
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 09\n");

    auto input = read_input(std::ifstream{"puzzle.in"});

    int window_size = 25;

    fmt::print("Part 1 Solution: {}\n", part1(input, window_size));
    fmt::print("Part 2 Solution: {}\n", part2(input, part1(input, window_size)));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
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

    SECTION("Can solve part 2 example") { REQUIRE(62 == part2(input, part1(input, window_size))); }
}

#endif
