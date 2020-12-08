
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <set>

namespace rv = ranges::views;

struct instruction {
    std::string name;
    int         amt;
};

class game_console {
public:
    struct instruction {
        std::string name;
        int         amt;
    };

    using program = std::vector<instruction>;

    struct result {
        int  acc_state;
        bool inf_loop_reached;
    };

public:
    game_console(std::vector<instruction> program)
        : program_{std::move(program)}
    {
    }

    result run()
    {
        std::set<int> cache;

        while (!inf_loop_detected_ && !program_terminate_) {
            auto& i = program_[next_instruction_];

            if (i.name == "acc") {
                accumulator_ += i.amt;
                ++next_instruction_;
            }
            else if (i.name == "jmp") {
                next_instruction_ += i.amt;
            }
            else if (i.name == "nop") {
                ++next_instruction_;
            }

            if (next_instruction_ >= program_.size()) {
                program_terminate_ = true;
            }
            else if (cache.contains(next_instruction_)) {
                inf_loop_detected_ = true;
            }
            else {
                cache.emplace(next_instruction_);
            }
        }

        return {accumulator_, inf_loop_detected_};
    }

private:
    program program_;
    int     accumulator_       = 0;
    int     next_instruction_  = 0;
    bool    program_terminate_ = false;
    bool    inf_loop_detected_ = false;
};

int part1(const game_console::program& p)
{
    game_console console{p};

    auto [acc, _] = console.run();

    return acc;
}

int part2(const game_console::program& p)
{
    bool found       = false;
    int  accumulator = 0, check = 0;

    while (!found) {
        while (p[check].name != "nop" && p[check].name != "jmp")
            ++check;

        game_console::program p_test = p;
        p_test[check].name           = (p_test[check].name == "jmp") ? "nop" : "jmp";

        ++check;

        game_console console{p_test};

        auto [acc, inf_loop] = console.run();

        if (!inf_loop) {
            found       = true;
            accumulator = acc;
        }
    }

    return accumulator;
}

int main()
{
    fmt::print("Advent of Code 2020 - Day 08\n");

    std::ifstream ifs{"days/day08/input.txt"};

    // clang-format off
    auto program = ranges::getlines(ifs) 
        | rv::transform([](auto&& s) {
            return game_console::instruction{s.substr(0, 3), std::stoi(s.substr(4))}; })
        | ranges::to<std::vector>;
    // clang-format on

    fmt::print("Part 1 Solution: {}\n", part1(program));
    fmt::print("Part 1 Solution: {}\n", part2(program));

    return 0;
}
