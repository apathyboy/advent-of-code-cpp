#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

std::unordered_map<uint32_t, uint32_t> distribution
    = {{3, 1}, {4, 3}, {5, 6}, {6, 7}, {7, 6}, {8, 3}, {9, 1}};

struct pawn_t {
    uint64_t score;
    uint64_t pos;

    friend auto operator<=>(const pawn_t&, const pawn_t&) = default;
};

template <>
struct std::hash<pawn_t> {
    std::size_t operator()(pawn_t const& p) const noexcept
    {
        std::size_t h1 = std::hash<int64_t>{}(p.score);
        std::size_t h2 = std::hash<int64_t>{}(p.pos);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

struct state_t {
    pawn_t player1 = {};
    pawn_t player2 = {};

    friend auto operator<=>(const state_t&, const state_t&) = default;
};


template <>
struct std::hash<state_t> {
    std::size_t operator()(state_t const& s) const noexcept
    {
        std::size_t h1 = std::hash<pawn_t>{}(s.player1);
        std::size_t h2 = std::hash<pawn_t>{}(s.player2);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

struct game_t {
    state_t state;

    int      deterministic_die = 0;
    uint64_t rolls             = 0;

    game_t(std::istream& input)
    {
        std::string tmp;

        std::getline(input, tmp);
        state.player1.pos = tmp.back() - '0';

        std::getline(input, tmp);
        state.player2.pos = tmp.back() - '0';
    }

    int roll()
    {
        ++rolls;

        ++deterministic_die;
        if (deterministic_die > 100) { deterministic_die = 1; }

        return deterministic_die;
    }

    void take_turn(pawn_t& p)
    {
        p.pos += roll() + roll() + roll();

        if (p.pos > 10) { p.pos = p.pos % 10 == 0 ? 10 : p.pos % 10; }

        p.score += p.pos;
    }
};

uint64_t ordinal_mod(uint64_t value, uint32_t max)
{
    return (value - 1) % max + 1;
}

std::unordered_map<state_t, std::pair<uint64_t, uint64_t>> cache;

std::pair<uint64_t, uint64_t> cnt_wins(
    uint64_t player1_score,
    uint64_t player2_score,
    uint64_t player1_pos,
    uint64_t player2_pos,
    bool     player1_turn)
{
    if (player1_turn) {
        if (player2_score >= 21) return {0, 1};
        state_t s{.player1 = {player1_score, player1_pos}, .player2 = {player2_score, player2_pos}};
        if (cache.contains(s)) { return cache[s]; }
    }
    else {
        if (player1_score >= 21) return {1, 0};
        state_t s{.player1 = {player2_score, player2_pos}, .player2 = {player1_score, player1_pos}};
        auto    cached = cache.find(s);
        if (cached != cache.end()) { return {cached->second.second, cached->second.first}; }
    }

    if (player1_turn) {
        std::pair<uint64_t, uint64_t> sum = {0, 0};
        for (auto [roll, cnt] : distribution) {
            uint64_t new_pos = ordinal_mod(player1_pos + roll, 10);
            auto [p1, p2]
                = cnt_wins(player1_score + new_pos, player2_score, new_pos, player2_pos, false);
            sum.first += cnt * p1;
            sum.second += cnt * p2;
        }
        state_t s{.player1 = {player1_score, player1_pos}, .player2 = {player2_score, player2_pos}};
        cache.insert(std::make_pair(s, sum));
        return sum;
    }
    else {
        std::pair<uint64_t, uint64_t> sum = {0, 0};
        for (auto [roll, cnt] : distribution) {
            uint64_t new_pos = ordinal_mod(player2_pos + roll, 10);
            auto [p1, p2] = cnt_wins(player1_score, player2_score + new_pos, player1_pos, new_pos, true);
            sum.first += cnt * p1;
            sum.second += cnt * p2;
        }
        state_t s{.player1 = {player2_score, player2_pos}, .player2 = {player1_score, player1_pos}};
        cache.insert(std::make_pair(s, std::make_pair(sum.second, sum.first)));
        return sum;
    }
}

uint64_t part1(game_t game)
{
    while (game.state.player1.score < 1000 && game.state.player2.score < 1000) {
        game.take_turn(game.state.player1);

        if (game.state.player1.score < 1000) { game.take_turn(game.state.player2); }
    }

    uint64_t losing_score = std::min(game.state.player1.score, game.state.player2.score);

    return losing_score * game.rolls;
}

uint64_t part2(game_t game)
{
    auto [p1, p2] = cnt_wins(
        game.state.player1.score,
        game.state.player2.score,
        game.state.player1.pos,
        game.state.player2.pos,
        true);

    return std::max(p1, p2);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 21\n");

    std::ifstream ifs{"puzzle.in"};
    game_t        game{ifs};

    fmt::print("Part 1 Solution: {}\n", part1(game));
    fmt::print("Part 2 Solution: {}\n", part2(game));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(Player 1 starting position: 4
Player 2 starting position: 8)";

    game_t game{ss};

    REQUIRE(739785 == part1(game));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(Player 1 starting position: 4
Player 2 starting position: 8)";

    game_t game{ss};

    REQUIRE(444356092776315 == part2(game));
}

#endif
