
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>

namespace rv = ranges::views;

struct instruction {
    std::string name;
    int         amt;
    bool        visited;
};

int main()
{
    fmt::print("Advent of Code 2020 - Day 08\n");

    std::ifstream ifs{"days/day08/input.txt"};

    auto instructions = ranges::getlines(ifs) | rv::transform([](auto&& s) {
                            return instruction{s.substr(0, 3), std::stoi(s.substr(4)), false};
                        })
                        | ranges::to<std::vector>;

    int  accumulator = 0, next_instruction = 0;
    bool inf_loop_detected = false;

    while (!inf_loop_detected) {
        auto& i = instructions[next_instruction];

        if (i.visited) {
            inf_loop_detected = true;
        }
        else {
            i.visited = true;

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
        }
    }

    fmt::print("Part 1 Solution: {}\n", accumulator);

    return 0;
}
