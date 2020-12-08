
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <map>
#include <set>

namespace rv = ranges::views;

class game_console {
public:
    enum class OP_TYPE : int { NOP = 0, JMP = 1, ACC = 2 };

    struct instruction {
        OP_TYPE op;
        int     amt;
    };

    using program = std::vector<instruction>;

    struct result {
        int  acc_state;
        bool inf_loop_reached;
    };

public:
    game_console(const std::vector<instruction>& program)
        : program_{program}
    {
    }

    result run()
    {
        std::set<int> cache;

        while (!inf_loop_detected_) {
            auto& i = program_[next_instruction_];

            switch (i.op) {
                case OP_TYPE::NOP: {
                    ++next_instruction_;
                } break;
                case OP_TYPE::JMP: {
                    next_instruction_ += i.amt;
                } break;
                case OP_TYPE::ACC: {
                    accumulator_ += i.amt;
                    ++next_instruction_;
                } break;
            }

            if (next_instruction_ >= program_.size())
                break;

            inf_loop_detected_ = cache.contains(next_instruction_);

            if (!inf_loop_detected_)
                cache.emplace(next_instruction_);
        }

        return {accumulator_, inf_loop_detected_};
    }

private:
    const program& program_;
    int            accumulator_       = 0;
    int            next_instruction_  = 0;
    bool           inf_loop_detected_ = false;
};

game_console::program read_input_program(std::istream&& i)
{
    std::map<std::string, game_console::OP_TYPE> op_map = {
        {"nop", game_console::OP_TYPE::NOP},
        {"jmp", game_console::OP_TYPE::JMP},
        {"acc", game_console::OP_TYPE::ACC}};
    ;

    // clang-format off
    return ranges::getlines(i) 
        | rv::transform([&op_map](auto&& s) {
            return game_console::instruction{op_map.at(s.substr(0, 3)), std::stoi(s.substr(4))}; })
        | ranges::to<std::vector>;
    // clang-format on
}

int part1(const game_console::program& p)
{
    game_console console{p};

    auto [acc, _] = console.run();

    return acc;
}

int part2(game_console::program p)
{
    bool found       = false;
    int  accumulator = 0, check = 0;

    while (!found) {
        while (p[check].op != game_console::OP_TYPE::NOP
               && p[check].op != game_console::OP_TYPE::JMP)
            ++check;

        // toggle check
        p[check].op = (p[check].op == game_console::OP_TYPE::NOP) ? game_console::OP_TYPE::JMP
                                                                  : game_console::OP_TYPE::NOP;

        game_console console{p};

        auto [acc, inf_loop] = console.run();

        if (!inf_loop) {
            found       = true;
            accumulator = acc;
        }

        // toggle back
        p[check].op = (p[check].op == game_console::OP_TYPE::NOP) ? game_console::OP_TYPE::JMP
                                                                  : game_console::OP_TYPE::NOP;

        ++check;
    }

    return accumulator;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 08\n");

    auto program = read_input_program(std::ifstream{"days/day08/input.txt"});

    fmt::print("Part 1 Solution: {}\n", part1(program));
    fmt::print("Part 1 Solution: {}\n", part2(std::move(program)));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 8 problems")
{
    std::stringstream ss;

    ss << R"(nop +0
acc +1
jmp +4
acc +3
jmp -3
acc -99
acc +1
jmp -4
acc +6)";

    auto program = read_input_program(std::move(ss));

    REQUIRE(9 == program.size());
    REQUIRE(game_console::OP_TYPE::ACC == program[5].op);
    REQUIRE(-99 == program[5].amt);


    SECTION("Can solve part 1 example") { REQUIRE(5 == part1(program)); }

    SECTION("Can solve part 2 example") { REQUIRE(8 == part2(program)); }
}

#endif
