
#include "intcode.hpp"

#include <fmt/format.h>

namespace aoc {

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

}