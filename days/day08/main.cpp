
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <set>

namespace rv = ranges::views;

struct instruction {
    std::string name;
    int         amt;
};

int part1(const std::vector<instruction>& instructions)
{
    int  accumulator = 0, next_instruction = 0;
    bool inf_loop_detected = false;

    std::set<int> cache;

    while (!inf_loop_detected) {
        auto& i = instructions[next_instruction];

        if (i.name == "acc") {
            accumulator += i.amt;
            ++next_instruction;
        }
        else if (i.name == "jmp") {
            next_instruction += i.amt;
        }
        else if (i.name == "nop") {
            ++next_instruction;
        }

        if (cache.contains(next_instruction)) {
            inf_loop_detected = true;
        }
        else {
            cache.emplace(next_instruction);
        }
    }

    return accumulator;
}

int part2(const std::vector<instruction>& instructions)
{
    bool program_terminate = false;
    int  accumulator       = 0;
    int  check             = 0;

    while (!program_terminate) {
        while (instructions[check].name != "nop" && instructions[check].name != "jmp"
               && check < instructions.size())
            ++check;

        int next_instruction   = 0;
        accumulator            = 0;
        bool inf_loop_detected = false;

        std::set<int> cache;

        while (!inf_loop_detected && !program_terminate) {
            auto& i = instructions[next_instruction];

            auto op = i.name;

            if (next_instruction == check) {
                op = (op == "jmp") ? "nop" : "jmp";
            }

            if (op == "acc") {
                accumulator += i.amt;
                ++next_instruction;
            }
            else if (op == "jmp") {
                next_instruction += i.amt;
            }
            else if (op == "nop") {
                ++next_instruction;
            }

            if (next_instruction >= instructions.size()) {
                program_terminate = true;
            }

            if (cache.contains(next_instruction)) {
                inf_loop_detected = true;
            }
            else {
                cache.emplace(next_instruction);
            }
        }

        ++check;
    }

    return accumulator;
}

int main()
{
    fmt::print("Advent of Code 2020 - Day 08\n");

    std::ifstream ifs{"days/day08/input.txt"};

    auto instructions = ranges::getlines(ifs) | rv::transform([](auto&& s) {
                            return instruction{s.substr(0, 3), std::stoi(s.substr(4))};
                        })
                        | ranges::to<std::vector>;

    fmt::print("Part 1 Solution: {}\n", part1(instructions));
    fmt::print("Part 1 Solution: {}\n", part2(instructions));

    return 0;
}
