
#include <aoc/aoc.hpp>
#include <aoc/intcode.hpp>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

int compute(std::vector<int> program, int noun, int verb)
{
    program[1] = noun;
    program[2] = verb;

    program = aoc::compute(program);

    return program[0];
}

auto compute_with_noun_and_verb(const std::vector<int>& program)
{
    return [&program](auto&& p) {
        auto [noun, verb] = p;
        return std::make_tuple(noun, verb, compute(program, noun, verb));
    };
}

auto keep_output_match(int keep_result)
{
    return [keep_result](const auto& t) {
        auto [_, __, result] = t;
        return result == keep_result;
    };
}

#ifndef UNIT_TESTING

int part1(std::vector<int> program)
{
    return compute(program, 12, 2);
}

int part2(std::vector<int> program)
{
    auto matches = rv::cartesian_product(rv::iota(0, 99), rv::iota(0, 99))
                   | rv::transform(compute_with_noun_and_verb(program))
                   | rv::filter(keep_output_match(19690720));

    auto [noun, verb, _] = rs::front(matches);

    return (100 * noun) + verb;
}

int main()
{
    fmt::print("Advent of Code 2019 - Day 02\n");

    std::ifstream ifs{"puzzle.in"};
    auto          program = aoc::split_line_by<int>(ifs, ',');

    fmt::print("Part 1 Solution: {}\n", part1(program));
    fmt::print("Part 2 Solution: {}\n", part2(program));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Can solve part 1 example") {}

TEST_CASE("Can solve part 2 example") {}

#endif
