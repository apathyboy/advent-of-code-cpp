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

int64_t transform_subject(int subject, int loop_size)
{
    return rs::accumulate(rv::iota(0, loop_size), int64_t{1}, [&subject](auto value, auto) {
        return (value * subject) % 20201227;
    });
}

int loops_to_reach(int subject, int target)
{
    int loops = 0;
    int value = 1;

    while (value != target) {
        value = (value * subject) % 20201227;
        ++loops;
    }

    return loops;
}

int64_t part1(std::vector<int> public_keys)
{
    return transform_subject(public_keys[1], loops_to_reach(7, public_keys[0]));
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 25\n");

    std::ifstream ifs{"days/day25/puzzle.in"};
    auto          public_keys = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(public_keys));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can determine loops to reach public key")
{
    REQUIRE(8 == loops_to_reach(7, 5764801));
    REQUIRE(11 == loops_to_reach(7, 17807724));
}

TEST_CASE("Can transform subject number")
{
    REQUIRE(5764801 == transform_subject(7, 8));
    REQUIRE(14897079 == transform_subject(17807724, 8));
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(5764801
17807724)";

    auto public_keys = read_input(ss);

    REQUIRE(14897079 == part1(public_keys));
}

#endif
