#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <list>

namespace rs = ranges;
namespace rv = ranges::views;

std::vector<int> parse_input1(const std::string& input, int size)
{
    std::vector<int> cups;
    cups.resize(size + 1);

    for (int i = 0; i < input.length() - 1; ++i) {
        cups[input[i] - '0'] = input[i + 1] - '0';
    }

    cups[rs::back(input) - '0'] = input[0] - '0';

    return cups;
}

std::vector<int> parse_input2(const std::string& input, int size)
{
    std::vector<int> cups;
    cups.resize(size + 1);

    int i = 0;
    for (; i < input.length() - 1; ++i) {
        cups[input[i] - '0'] = input[i + 1] - '0';
    }
    cups[input[i] - '0'] = 10;
    for (i = 10; i <= size; ++i) {
        cups[i] = i + 1;
    }
    cups[size] = input[0] - '0';

    return cups;
}

std::vector<int> play(std::vector<int> cups, int starting_cup, int moves)
{
    int current_cup = starting_cup;

    rs::for_each(rv::iota(0, moves), [&cups, &current_cup](auto) {
        int pick1       = cups[current_cup];
        int pick2       = cups[pick1];
        int pick3       = cups[pick2];
        int destination = current_cup - 1;

        if (destination <= 0) { destination = static_cast<int>(cups.size() - 1); }

        while (destination == pick1 || destination == pick2 || destination == pick3) {
            --destination;
            if (destination <= 0) { destination = static_cast<int>(cups.size() - 1); }
        }

        cups[current_cup] = cups[pick3];
        int tmp           = cups[destination];
        cups[destination] = pick1;
        cups[pick3]       = tmp;
        current_cup       = cups[current_cup];
    });

    return cups;
}

std::string part1(const std::string& input)
{
    auto cups = parse_input1(input, static_cast<int>(input.size()));

    cups = play(cups, input[0] - '0', 100);

    std::string answer = "";
    int         i      = 1;
    do {
        answer.push_back(static_cast<char>(cups[i] + '0'));
        i = cups[i];
    } while (i != 1);

    return answer | rv::drop_last(1) | rs::to<std::string>;
}

int64_t part2(const std::string& input)
{
    auto cups = parse_input2(input, 1000000);

    cups = play(cups, input[0] - '0', 10000000);

    return static_cast<int64_t>(cups[1]) * static_cast<int64_t>(cups[cups[1]]);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 23\n");

    std::string input = "589174263";

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    REQUIRE(std::string{"67384529"} == part1("389125467"));
}

TEST_CASE("Can solve part 2 example")
{
    REQUIRE(149245887792 == part2("389125467"));
}

#endif
