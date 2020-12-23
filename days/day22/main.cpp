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

std::deque<int> play_combat(std::vector<std::deque<int>> decks)
{
    do {
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
    } while (!winner_detected(decks));

    return decks[decks[0].size() == 0 ? 1 : 0];
}

std::pair<int, std::deque<int>> play_recursive_combat(std::vector<std::deque<int>> decks)
{
    std::vector<std::vector<std::deque<int>>> previous_states;

    do {
        if (rs::any_of(previous_states, [&decks](const auto& state) { return state == decks; })) {
            return std::make_pair(0, decks[0]);
        }

        previous_states.push_back(decks);

        auto p1 = decks[0].front();
        decks[0].pop_front();

        auto p2 = decks[1].front();
        decks[1].pop_front();

        int winner = 0;

        if (p1 <= decks[0].size() && p2 <= decks[1].size()) {
            std::vector<std::deque<int>> subgame_decks{
                decks[0] | rv::take(p1) | rs::to<std::deque>,
                decks[1] | rv::take(p2) | rs::to<std::deque>};

            winner = play_recursive_combat(subgame_decks).first;
        }
        else {
            winner = (p1 > p2) ? 0 : 1;
        }

        if (winner == 0) {
            decks[0].push_back(p1);
            decks[0].push_back(p2);
        }
        else {
            decks[1].push_back(p2);
            decks[1].push_back(p1);
        }
    } while (!winner_detected(decks));

    int winner = decks[0].size() == 0 ? 1 : 0;

    return std::make_pair(winner, decks[winner]);
}

int64_t part1(std::vector<std::deque<int>> decks)
{
    return calculate_deck_score(play_combat(decks));
}

int64_t part2(std::vector<std::deque<int>> decks)
{
    return calculate_deck_score(play_recursive_combat(decks).second);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 22\n");

    std::string input_path = "days/day22/puzzle.in";

    auto decks = read_starting_decks(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1(decks));
    fmt::print("Part 2 Solution: {}\n", part2(decks));

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

    REQUIRE(291 == part2(decks));
}

#endif
