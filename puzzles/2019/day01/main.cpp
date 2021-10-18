#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;

int fuel_for_mass(int mass)
{
    return (mass / 3) - 2;
}

int fuel_for_module(int module_mass)
{
    // clang-format off
    return rs::accumulate(
        rv::iota(0) 
            | rv::exclusive_scan(module_mass, [](auto i, auto) { return fuel_for_mass(i); })
            | rv::tail 
            | rv::take_while([](int i) { return i > 0; }),
        0);
    // clang-format on
}

int part1(std::vector<int>& module_masses)
{
    return rs::accumulate(module_masses | rv::transform(fuel_for_mass), 0);
}

int part2(std::vector<int>& module_masses)
{
    return rs::accumulate(module_masses | rv::transform(fuel_for_module), 0);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2019 - Day 01\n");

    auto module_masses = aoc::read_int_per_line(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(module_masses));
    fmt::print("Part 2 Solution: {}\n", part2(module_masses));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Calculate fuel for mass")
{
    REQUIRE(2 == fuel_for_mass(12));
    REQUIRE(2 == fuel_for_mass(14));
    REQUIRE(654 == fuel_for_mass(1969));
    REQUIRE(33583 == fuel_for_mass(100756));
}

TEST_CASE("Calculate fuel for module")
{
    REQUIRE(2 == fuel_for_module(14));
    REQUIRE(966 == fuel_for_module(1969));
    REQUIRE(50346 == fuel_for_module(100756));
}

#endif