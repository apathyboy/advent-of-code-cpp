
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

struct board_position_t {
    int  number;
    bool marked = false;
};

typedef std::vector<std::vector<board_position_t>> board_t;

typedef std::vector<int> draw_numbers_t;

struct bingo_subsystem_t {
    draw_numbers_t       draw_numbers;
    std::vector<board_t> boards;
};

board_t read_board(std::istream& input)
{
    board_t board{};

    std::string tmp;
    std::getline(input, tmp);

    for (int i = 0; i < 5; ++i) {
        std::getline(input, tmp);

        size_t begin_pos = 0;
        size_t end_pos   = 0;

        std::vector<board_position_t> row;

        for (int j = 0; j < 5; ++j) {
            board_position_t bp{};

            if (tmp[begin_pos] == ' ') ++begin_pos;
            end_pos = tmp.find(' ', begin_pos);

            bp.number = std::stoi(tmp.substr(begin_pos, end_pos));

            begin_pos = end_pos + 1;

            row.emplace_back(bp);
        }

        board.emplace_back(std::move(row));
    }

    return board;
}

bingo_subsystem_t read_input(std::istream&& input)
{
    bingo_subsystem_t bingo_subsystem{};

    std::string tmp;
    std::getline(input, tmp);

    bingo_subsystem.draw_numbers = tmp | rv::split(',')
                                   | rv::transform([](auto&& s) { return s | rs::to<std::string>; })
                                   | rv::transform([](auto&& s) { return std::stoi(s); })
                                   | rs::to<std::vector>;

    while (!input.eof()) {
        bingo_subsystem.boards.emplace_back(read_board(input));
    }

    return bingo_subsystem;
}

void mark_number(board_t& board, int num)
{
    for (auto& row : board) {
        for (auto& pos : row) {
            if (pos.number == num) pos.marked = true;
        }
    }
}

bool board_won(const board_t& board)
{
    for (auto row : board) {
        bool won = true;
        for (auto pos : row) {
            if (!pos.marked) won = false;
        }

        if (won) return true;
    }

    for (int i = 0; i < board[0].size(); ++i) {
        bool won = true;
        for (auto row : board) {
            if (!row[i].marked) won = false;
        }

        if (won) return true;
    }

    return false;
}

int board_score(const board_t& board)
{
    int score = 0;

    for (auto row : board) {
        for (auto pos : row) {
            if (!pos.marked) { score += pos.number; }
        }
    }

    return score;
}

int part1(bingo_subsystem_t bingo_subsystem)
{
    int min_drawn_numbers = std::numeric_limits<int>::max();
    int winning_number    = 0;
    int winning_score     = 0;

    for (auto board : bingo_subsystem.boards) {
        for (int i = 0; i < bingo_subsystem.draw_numbers.size(); ++i) {
            int drawn_number = bingo_subsystem.draw_numbers[i];

            mark_number(board, drawn_number);

            if (board_won(board)) {
                if (i < min_drawn_numbers) {
                    min_drawn_numbers = i;
                    winning_number    = drawn_number;
                    winning_score     = board_score(board);
                }

                break;
            }
        }
    }

    return winning_score * winning_number;
}

int part2(bingo_subsystem_t bingo_subsystem)
{
    int max_drawn_numbers = 0;
    int winning_number    = 0;
    int winning_score     = 0;

    for (auto board : bingo_subsystem.boards) {
        for (int i = 0; i < bingo_subsystem.draw_numbers.size(); ++i) {
            int drawn_number = bingo_subsystem.draw_numbers[i];

            mark_number(board, drawn_number);

            if (board_won(board)) {
                if (i > max_drawn_numbers) {
                    max_drawn_numbers = i;
                    winning_number    = drawn_number;
                    winning_score     = board_score(board);
                }

                break;
            }
        }
    }

    return winning_score * winning_number;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 04\n");

    auto input = read_input(std::ifstream{"puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7)";

    auto bingo_subsystem = read_input(std::move(ss));
    REQUIRE(4512 == part1(bingo_subsystem));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(7,4,9,5,11,17,23,2,0,14,21,24,10,16,13,6,15,25,12,22,18,20,8,19,3,26,1

22 13 17 11  0
 8  2 23  4 24
21  9 14 16  7
 6 10  3 18  5
 1 12 20 15 19

 3 15  0  2 22
 9 18 13 17  5
19  8  7 25 23
20 11 10 24  4
14 21 16 12  6

14 21 17 24  4
10 16 15  9 19
18  8 23 26 20
22 11 13  6  5
 2  0 12  3  7)";

    auto bingo_subsystem = read_input(std::move(ss));
    REQUIRE(1924 == part2(bingo_subsystem));
}

#endif