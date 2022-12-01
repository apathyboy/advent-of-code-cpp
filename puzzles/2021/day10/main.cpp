#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <stack>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

int score_error(char c)
{
    return (')' == c) ? 3 : (']' == c) ? 57 : ('}' == c) ? 1197 : ('>' == c) ? 25137 : 0;
}

int score_completion_char(char c)
{
    return (')' == c) ? 1 : (']' == c) ? 2 : ('}' == c) ? 3 : ('>' == c) ? 4 : 0;
}

char get_open(char c)
{
    return (')' == c) ? '(' : (']' == c) ? '[' : ('}' == c) ? '{' : ('>' == c) ? '<' : ' ';
}

char get_close(char c)
{
    return ('(' == c) ? ')' : ('[' == c) ? ']' : ('{' == c) ? '}' : ('<' == c) ? '>' : ' ';
}

int find_syntax_error(const std::string& line)
{
    std::stack<char> chars{};

    int score = 0;

    for (char c : line) {
        if (c == '(' || c == '[' || c == '{' || c == '<') chars.push(c);
        else {
            char tmp = chars.top();
            chars.pop();

            if (tmp != get_open(c)) {
                score = score_error(c);
                break;
            }
        }
    }

    return score;
}


std::string finish_line(const std::string& line)
{
    std::stack<char> chars{};

    for (char c : line) {
        if (c == '(' || c == '[' || c == '{' || c == '<') chars.push(c);
        else {
            char tmp = chars.top();
            if (tmp == get_open(c)) { chars.pop(); }
        }
    }

    std::string completion{};

    while (!chars.empty()) {
        char tmp = chars.top();
        chars.pop();

        completion.push_back(get_close(tmp));
    }

    return completion;
}

int64_t score_line_completion(const std::string& line)
{
    int64_t score = 0;

    for (char c : line) {
        score *= 5;
        score += score_completion_char(c);
    }

    return score;
}

std::vector<std::string> read_input(std::istream& input)
{
    return rs::getlines(input) | rs::to<std::vector>;
}

int part1(const std::vector<std::string>& input)
{
    return rs::accumulate(input | rv::transform(find_syntax_error), 0);
}

int64_t part2(const std::vector<std::string>& input)
{
    auto scores = input | rv::filter([](const auto& s) { return 0 == find_syntax_error(s); })
                  | rv::transform(finish_line) | rv::transform(score_line_completion)
                  | rs::to<std::vector> | ra::sort;

    return scores[scores.size() / 2];
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 10\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"([({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]])";

    auto input = read_input(ss);

    REQUIRE(26397 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"([({(<(())[]>[[{[]{<()<>>
[(()[<>])]({[<{<<[]>>(
{([(<{}[<>[]}>{[]{[(<()>
(((({<>}<{<{<>}{[]{[]{}
[[<[([]))<([[{}[[()]]]
[{[{({}]{}}([{[{{{}}([]
{<[[]]>}<{[{[{[]{()[[[]
[<(<(<(<{}))><([]([]()
<{([([[(<>()){}]>(<<{{
<{([{{}}[<[[[<>{}]]]>[]])";

    auto input = read_input(ss);

    REQUIRE(288957 == part2(input));
}

#endif
