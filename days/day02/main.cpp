
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp> // get everything

#include <fstream>
#include <iostream>

using namespace ranges;

using namespace aoc2020;

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day02 <input_file_path>\n");
    }

    {
        std::ifstream ifs{argv[1]};

        auto parse     = [](const auto& s) { return parse_password_rule_string(s); };
        auto selection = [](const auto& entry) {
            auto c = count(entry.second, entry.first.target);
            return (c >= entry.first.min_count && c <= entry.first.max_count);
        };

        auto valid = ranges::getlines(ifs) | views::transform(parse)
                     | views::filter(selection);

        fmt::print("Part 1 Solution: {}\n", distance(valid));
    }

    {
        std::ifstream ifs{argv[1]};

        auto parse     = [](const auto& s) { return parse_password_rule_string(s); };
        auto selection = [](const auto& entry) {
            const auto& str    = entry.second;
            int         pos1   = entry.first.min_count - 1;
            int         pos2   = entry.first.max_count - 1;
            char        target = entry.first.target;

            return (str[pos1] == target && str[pos2] != target)
                   || (str[pos1] != target && str[pos2] == target);
        };

        auto valid = ranges::getlines(ifs) | views::transform(parse)
                     | views::filter(selection);

        fmt::print("Part 2 Solution: {}\n", distance(valid));
    }

    return 0;
}
