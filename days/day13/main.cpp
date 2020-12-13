#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <string>

namespace rs = ranges;
namespace rv = ranges::views;

std::pair<int, std::vector<int>> read_input(std::istream&& input)
{
    std::string tmp;
    std::getline(input, tmp);

    int earliest_departure = std::stoi(tmp);

    std::getline(input, tmp);

    auto schedules = tmp | rv::split(',')
                     | rv::filter([](const auto& rng) { return rs::front(rng) != 'x'; })
                     | rv::transform([](auto&& rng) {
                           return std::stoi(std::string(&*rng.begin(), rs::distance(rng)));
                       })
                     | rs::to_vector;

    return {earliest_departure, schedules};
}

int part1(int earliest_departure, const std::vector<int>& schedules)
{
    int                found_departure = earliest_departure;
    std::optional<int> result;

    while (!result) {
        for (auto bus : schedules) {
            if (found_departure % bus == 0) { result = (found_departure - earliest_departure) * bus; }
        }

        ++found_departure;
    }

    return result.value();
}

int part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 13\n");

    auto [earliest_departure, schedules] = read_input(std::ifstream{"days/day13/puzzle.in"});

    fmt::print("Earliest Departure: {}\n", earliest_departure);
    fmt::print("Schedules: [{}]\n", fmt::join(schedules, ","));

    fmt::print("Part 1 Solution: {}\n", part1(earliest_departure, schedules));
    fmt::print("Part 2 Solution: {}\n", part2());

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

    SECTION("Can solve part 2 example") { REQUIRE(0 == part2()); }
}

#endif
