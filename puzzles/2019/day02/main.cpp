
#include <aoc/aoc.hpp>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

std::vector<int> compute(std::vector<int> program)
{
    int instruction_pointer = 0;

    while (program[instruction_pointer] != 99) {
        switch (program[instruction_pointer]) {
            case 1: {
                program[program[instruction_pointer + 3]] = program[program[instruction_pointer + 1]]
                                                            + program[program[instruction_pointer + 2]];
                instruction_pointer += 4;
            } break;
            case 2: {
                program[program[instruction_pointer + 3]] = program[program[instruction_pointer + 1]]
                                                            * program[program[instruction_pointer + 2]];
                instruction_pointer += 4;
            } break;
            default: {
                std::runtime_error{fmt::format("Invalid Opcode: {}", program[instruction_pointer])};
            }
        }
    }

    return program;
}

int compute(std::vector<int> program, int noun, int verb)
{
    program[1] = noun;
    program[2] = verb;

    program = compute(program);

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

    auto program = aoc::read_ints(std::ifstream{"puzzle.in"}, ',');

    fmt::print("Part 1 Solution: {}\n", part1(program));
    fmt::print("Part 2 Solution: {}\n", part2(program));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Can run simple intcode program")
{
    std::vector<int> program{1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50};

    program = compute(program);

    REQUIRE(3500 == program[0]);
}

#endif