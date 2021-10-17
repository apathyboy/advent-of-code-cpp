#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <sstream>
#include <string>

namespace rs = ranges;
namespace rv = ranges::views;

int64_t solve_expression(std::istream& expr)
{
    int64_t solution = 0;

    int64_t num;
    char    op = ' ', dummy;

    while (!(expr >> std::ws).eof()) {
        char c = static_cast<char>((expr >> std::ws).peek());
        switch (c) {
            case '(': {
                expr >> dummy;

                num = solve_expression(expr);

                if (op == '+') solution += num;
                else if (op == '*')
                    solution *= num;
                else
                    solution = num;
            } break;
            case ')': {
                expr >> dummy;
                return solution;
            } break;
            case '+':
            case '*': {
                expr >> op;
            } break;
            default: {
                expr >> num;

                if (op == '+') solution += num;
                else if (op == '*')
                    solution *= num;
                else
                    solution = num;
            } break;
        }
    }

    return solution;
}


int64_t solve_advanced_expression(std::istream& expr)
{
    int64_t solution = 0;

    int64_t num;
    char    op = ' ', dummy;

    while (!(expr >> std::ws).eof()) {
        char c = static_cast<char>((expr >> std::ws).peek());
        switch (c) {
            case '(': {
                expr >> dummy;

                num = solve_advanced_expression(expr);

                if (op == '+') solution += num;
                else if (op == '*')
                    solution *= solve_advanced_expression(expr);
                else
                    solution = num;
            } break;
            case ')': {
                expr >> dummy;
                return solution;
            } break;
            case '+': {
                expr >> op;
            } break;
            case '*': {
                expr >> op;
                solution *= solve_advanced_expression(expr);
                return solution;
            } break;
            default: {
                expr >> num;

                if (op == '+') solution += num;
                else if (op == '*')
                    solution *= solve_advanced_expression(expr);
                else
                    solution = num;
            } break;
        }
    }

    return solution;
}

int64_t part1(std::istream&& input)
{
    auto solutions = rs::getlines(input) | rv::transform([](auto&& s) {
                         std::stringstream ss{s | rs::to<std::string>};
                         return solve_expression(ss);
                     });

    return rs::accumulate(solutions, int64_t{0});
}

int64_t part2(std::istream&& input)
{
    auto solutions = rs::getlines(input) | rv::transform([](auto&& s) {
                         std::stringstream ss{s | rs::to<std::string>};
                         return solve_advanced_expression(ss);
                     });

    return rs::accumulate(solutions, int64_t{0});
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 18\n");

    std::string input_path = "days/day18/puzzle.in";

    fmt::print("Part 1 Solution: {}\n", part1(std::ifstream{input_path}));
    fmt::print("Part 2 Solution: {}\n", part2(std::ifstream{input_path}));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve simple expressions")
{
    std::string       e = "1 + 2 * 3 + 4 * 5 + 6";
    std::stringstream ss{e};
    REQUIRE(71 == solve_expression(ss));
}

TEST_CASE("Can solve expressions with 1 nesting")
{
    {
        std::string       e = "2 * 3 + (4 * 5)";
        std::stringstream ss{e};
        REQUIRE(26 == solve_expression(ss));
    }
    {
        std::string       e = "5 + (8 * 3 + 9 + 3 * 4 * 3)";
        std::stringstream ss{e};
        REQUIRE(437 == solve_expression(ss));
    }
}

TEST_CASE("Can solve expressions with multiple nestings")
{
    std::string       e = "1 + (2 * 3) + (4 * (5 + 6))";
    std::stringstream ss{e};
    REQUIRE(51 == solve_expression(ss));
}

TEST_CASE("Can solve expressions that start with nestings")
{
    std::string       e = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
    std::stringstream ss{e};
    REQUIRE(13632 == solve_expression(ss));
}

TEST_CASE("Can solve simple expression using advanced math")
{
    std::string       e = "1 + 2 * 3 + 4 * 5 + 6";
    std::stringstream ss{e};
    REQUIRE(231 == solve_advanced_expression(ss));
}

TEST_CASE("Can solve expression with 1 nesting using advanced math")
{
    {
        std::string       e = "5 + (8 * 3 + 9 + 3 * 4 * 3)";
        std::stringstream ss{e};
        REQUIRE(1445 == solve_advanced_expression(ss));
    }
    {
        std::string       e = "2 * 3 + (4 * 5)";
        std::stringstream ss{e};
        REQUIRE(46 == solve_advanced_expression(ss));
    }
}

TEST_CASE("Can solve advanced expressions with multiple nestings")
{
    {
        std::string       e = "1 + (2 * 3) + (4 * (5 + 6))";
        std::stringstream ss{e};
        REQUIRE(51 == solve_advanced_expression(ss));
    }
    {
        std::string       e = "((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2";
        std::stringstream ss{e};
        REQUIRE(23340 == solve_advanced_expression(ss));
    }
}

TEST_CASE("Can solve advanced expressions with multiple subsequent multiplications")
{
    std::string       e = "5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))";
    std::stringstream ss{e};
    REQUIRE(669060 == solve_advanced_expression(ss));
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(2 * 3 + (4 * 5)
5 + (8 * 3 + 9 + 3 * 4 * 3)
5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4)))";

    REQUIRE(12703 == part1(std::move(ss)));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(2 * 3 + (4 * 5)
5 + (8 * 3 + 9 + 3 * 4 * 3)
5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4)))";

    REQUIRE(670551 == part2(std::move(ss)));
}

#endif
