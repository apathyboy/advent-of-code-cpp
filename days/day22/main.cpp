#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <deque>
#include <fstream>
#include <iostream>
#include <optional>

namespace rs = ranges;
namespace rv = ranges::views;

std::vector<std::deque<int>> read_starting_decks(std::istream&& input)
{
    return rs::getlines(input) | rv::split("") | rv::transform([](auto&& rng) {
               return rng | rv::tail
                      | rv::transform([](auto&& s) { return std::stoi(s | rs::to<std::string>); })
                      | rs::to<std::deque>;
           })
           | rs::to_vector;
}

int calculate_deck_score(const std::deque<int>& deck)
{
    return rs::accumulate(
        deck | rv::reverse | rv::enumerate | rv::transform([](auto&& p) {
            auto [idx, card_value] = p;
            return static_cast<int>(idx + 1) * card_value;
        }),
        0);
}

bool winner_detected(const std::vector<std::deque<int>>& decks)
{
    return rs::any_of(decks, [](const auto& deck) { return deck.size() == 0; });
}

std::optional<std::deque<int>> find_winner(const std::vector<std::deque<int>>& decks)
{
    std::optional<std::deque<int>> winner;

    auto winners = decks | rv::filter([](const auto& deck) { return deck.size() > 0; });

    if (rs::distance(winners) > 0) { winner = rs::front(winners); }

    return winner;
}

int64_t part1(std::vector<std::deque<int>> decks)
{
    std::optional<std::deque<int>> winner;

    int round = 0;

    do {
        ++round;

        auto p1 = decks[0].front();
        decks[0].pop_front();

        auto p2 = decks[1].front();
        decks[1].pop_front();

        if (p1 > p2) {
            decks[0].push_back(p1);
            decks[0].push_back(p2);
        }
        else {
            decks[1].push_back(p2);
            decks[1].push_back(p1);
        }

        winner = find_winner(decks);
    } while (!winner_detected(decks));

    return calculate_deck_score(winner.value());
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 22\n");

    std::string input_path = "days/day22/puzzle.in";

    auto decks = read_starting_decks(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1(decks));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can calculate deck score")
{
    std::deque<int> deck{3, 2, 10, 6, 8, 5, 9, 4, 7, 1};

    REQUIRE(306 == calculate_deck_score(deck));
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(Player 1:
9
2
6
3
1

Player 2:
5
8
4
7
10)";

    auto decks = read_starting_decks(std::move(ss));

    REQUIRE(306 == part1(decks));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
