
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <set>
#include <string>

namespace rv = ranges::views;

int64_t part1(const std::vector<std::string>& input)
{
    std::set<char> q1;
    int64_t        sum = 0;

    for (const auto& str : input) {
        if (str.length() == 0) {
            sum += q1.size();
            q1.clear();
        }

        ranges::insert(q1, str);
    }

    return sum;
}

int64_t part2()
{
    int64_t sum = 0;

    std::string str;
    std::string questions;

    std::ifstream ifs{"days/day06/input.txt"};

    std::getline(ifs, questions);
    ranges::sort(questions);

    while (std::getline(ifs, str)) {
        ranges::sort(str);

        if (str.length() == 0) {
            sum += static_cast<int>(questions.size());

            std::getline(ifs, questions);
            ranges::sort(questions);
        }
        else {
            questions = rv::set_intersection(questions, str) | ranges::to<std::string>;
        }
    }

    return sum;
}

int main()
{
    fmt::print("Advent of Code 2020 - Day 06\n");


    std::ifstream ifs{"days/day06/input.txt"};

    auto input = ranges::getlines(ifs) | ranges::to<std::vector>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}
