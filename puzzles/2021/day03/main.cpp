
#include <aoc/aoc.hpp>

#include <fmt/core.h>

#include <fstream>
#include <string>
#include <vector>

int filter_values(std::vector<std::string> input, bool most_common = true)
{
    int i = 0;
    while (input.size() > 1) {
        int ones = 0;

        for (auto s : input) {
            if (s[i] == '1') ++ones;
        }

        bool check = most_common ? (ones >= input.size() - ones) : (ones < input.size() - ones);

        input.erase(
            std::remove_if(
                input.begin(),
                input.end(),
                [i, check](const std::string& t) { return t[i] == (check ? '0' : '1'); }),
            input.end());

        ++i;
        if (i > input[0].length() - 1) i = 0;
    }

    return std::stoi(input[0], nullptr, 2);
}

int part1(const std::vector<std::string>& input)
{
    std::vector<int> counts(input[0].length());

    for (auto s : input) {
        for (int i = 0; i < s.length(); ++i) {
            if (s[i] == '1') ++counts[i];
        }
    }

    std::string gamma(counts.size(), '0');
    std::string epsilon(counts.size(), '0');

    size_t half = input.size() / 2;

    for (int i = 0; i < counts.size(); ++i) {
        if (counts[i] < half) { epsilon[i] = '1'; }
        else {
            gamma[i] = '1';
        }
    }

    return std::stoi(gamma, nullptr, 2) * std::stoi(epsilon, nullptr, 2);
}


int part2(std::vector<std::string> input)
{
    int oxygen_rating = filter_values(input);
    int c02_rating    = filter_values(input, false);

    return oxygen_rating * c02_rating;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 03\n");

    auto input = aoc::read_element_per_line<std::string>(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010)";

    auto input = aoc::read_element_per_line<std::string>(std::move(ss));

    REQUIRE(198 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(00100
11110
10110
10111
10101
01111
00111
11100
10000
11001
00010
01010)";

    auto input = aoc::read_element_per_line<std::string>(std::move(ss));

    REQUIRE(230 == part2(input));
}

#endif