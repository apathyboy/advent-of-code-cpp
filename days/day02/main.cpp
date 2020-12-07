
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>

using namespace ranges;

using namespace aoc2020;

int main()
{
    auto parse      = [](const auto& s) { return parse_password_rule_string(s); };
    auto selection1 = [](const auto& entry) { return is_valid_day2_part1_pw(entry); };
    auto selection2 = [](const auto& entry) { return is_valid_day2_part2_pw(entry); };

    {
        std::ifstream ifs{"days/day02/input.txt"};

        auto valid = getlines(ifs) | views::transform(parse) | views::filter(selection1);

        fmt::print("Part 1 Solution: {}\n", distance(valid));
    }

    {
        std::ifstream ifs{"days/day02/input.txt"};

        auto valid = getlines(ifs) | views::transform(parse) | views::filter(selection2);

        fmt::print("Part 2 Solution: {}\n", distance(valid));
    }

    return 0;
}
