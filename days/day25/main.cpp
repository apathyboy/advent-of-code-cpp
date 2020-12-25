#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

std::vector<int> read_input(std::istream& input)
{
    return rs::getlines(input)
           | rv::transform([](auto&& s) { return std::stoi(s | rs::to<std::string>); }) | rs::to_vector;
}

int64_t transform_subject(int subject, int loop_size, int64_t denominator)
{
    int64_t value = 1;

    for (int i = 0; i < loop_size; ++i) {
        value *= subject;
        auto [_, r] = std::div(value, denominator);
        value       = r;
    }

    return value;
}

int loops_to_reach(int subject, int denominator, int target)
{
    int i     = 0;
    int value = 1;

    while (value != target) {
        value *= subject;
        auto [_, r] = std::div(value, denominator);
        value       = r;
        ++i;
    }

    return i;
}

int64_t part1(std::vector<int> public_keys)
{
    int key1_loops = loops_to_reach(7, 20201227, public_keys[0]);
    int key2_loops = loops_to_reach(7, 20201227, public_keys[1]);


    return transform_subject(public_keys[0], key2_loops, 20201227);
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 25\n");

    std::ifstream ifs{"days/day25/puzzle.in"};
    auto          public_keys = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(public_keys));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can determine loops to reach public key")
{
    REQUIRE(8 == loops_to_reach(7, 20201227, 5764801));
    REQUIRE(11 == loops_to_reach(7, 20201227, 17807724));
}

TEST_CASE("Can transform subject number")
{
    REQUIRE(5764801 == transform_subject(7, 8, 20201227));
    REQUIRE(14897079 == transform_subject(17807724, 8, 20201227));
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(5764801
17807724)";

    auto public_keys = read_input(ss);

    REQUIRE(14897079 == part1(public_keys));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
