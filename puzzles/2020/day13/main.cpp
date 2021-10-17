#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

std::pair<int, std::vector<int>> read_input(std::istream&& input)
{
    std::string tmp;
    std::getline(input, tmp);

    int earliest_departure = std::stoi(tmp);

    std::getline(input, tmp);

    auto schedules = tmp | rv::split(',')
                     | rv::transform([](auto&& s) { return s | rs::to<std::string>; })
                     | rv::transform([](auto&& s) { return s == "x" ? 0 : std::stoi(s); })
                     | rs::to_vector;

    return {earliest_departure, schedules};
}

int part1(int earliest_departure, const std::vector<int>& schedules)
{
    // clang-format off
    auto wait_times = schedules 
        | rv::filter([](int i) { return i > 0; })
        | rv::transform([&earliest_departure](int i) {
            return std::make_pair(i - (earliest_departure % i), i); });
    // clang-format on

    auto [wait, bus] = rs::min(wait_times, [](const auto& a, const auto& b) {
        return a.first < b.first;
    });

    return wait * bus;
}

int64_t part2(const std::vector<int>& schedules)
{
    int64_t timestamp = 0;
    int64_t delta     = rs::front(schedules);

    auto check_schedules = rv::enumerate(schedules) | rv::tail
                           | rv::filter([](const auto& p) { return p.second != 0; });

    for (auto [idx, bus] : check_schedules) {
        while ((timestamp + idx) % bus != 0) {
            timestamp += delta;
        }

        delta *= bus;
    }

    return timestamp;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 13\n");

    auto [earliest_departure, schedules] = read_input(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(earliest_departure, schedules));
    fmt::print("Part 2 Solution: {}\n", part2(schedules));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 13 problems")
{
    std::stringstream ss;

    ss << R"(939
7,13,x,x,59,x,31,19)";

    auto [earliest_departure, schedules] = read_input(std::move(ss));

    SECTION("Can solve part 1 example") { REQUIRE(295 == part1(earliest_departure, schedules)); }

    SECTION("Can solve part 2 example") { REQUIRE(1068781 == part2(schedules)); }
}

#endif
