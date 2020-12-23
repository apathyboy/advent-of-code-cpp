#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <list>

namespace rs = ranges;
namespace rv = ranges::views;

std::list<int> digits_to_list(const std::string& s)
{
    return s | rv::transform([](char c) { return static_cast<int>(c - '0'); }) | rs::to<std::list>;
}

std::string list_to_digits(const std::list<int>& l)
{
    return l | rv::transform([](int i) { return static_cast<char>('0' + i); }) | rs::to<std::string>;
}

auto calculate_destination(int current_cup, std::list<int> picked_up, int min_cup, int max_cup)
{
    int destination = current_cup - 1;

    if (destination < min_cup) { destination = max_cup; }

    while (rs::contains(picked_up, destination)) {
        --destination;
        if (destination < min_cup) { destination = max_cup; }
    }

    return destination;
}

std::list<int> pick_up(std::list<int>& cups, int current_cup, int cups_to_pick_up = 3)
{
    auto           current_cup_iter = rs::find(cups, current_cup);
    std::list<int> picked_up        = cups | rv::cycle
                               | rv::drop(rs::distance(cups.begin(), current_cup_iter) + 1)
                               | rv::take(cups_to_pick_up) | rs::to<std::list>;

    cups = cups | rv::filter([&picked_up](auto i) {
               return rs::all_of(picked_up, [i](int picked_up) { return picked_up != i; });
           })
           | rs::to<std::list>;

    return picked_up;
}

void move_cups(std::list<int>& cups, std::list<int> picked_up, int destination)
{
    auto placement = rs::find(cups, destination);
    rs::advance(placement, 1);
    rs::insert(cups, placement, picked_up);
}

int select_new_current(const std::list<int>& cups, int current_cup)
{
    auto current_cup_iter = rs::find(cups, current_cup);
    auto next             = cups | rv::cycle | rv::drop(rs::distance(cups.begin(), current_cup_iter) + 1)
                | rv::take(1);

    return rs::front(next);
}

std::string calculate_cup_order(const std::list<int>& cups)
{
    auto cup_iter     = rs::find(cups, 1);
    auto ordered_cups = cups | rv::cycle | rv::drop(rs::distance(cups.begin(), cup_iter) + 1)
                        | rv::take(8) | rs::to<std::list>;

    return list_to_digits(ordered_cups);
}

std::string part1(std::list<int> cups, int moves = 100)
{
    int min_cup     = rs::min(cups);
    int max_cup     = rs::max(cups);
    int current_cup = rs::front(cups);

    while (moves > 0) {
        auto picked_up   = pick_up(cups, current_cup);
        int  destination = calculate_destination(current_cup, picked_up, min_cup, max_cup);
        move_cups(cups, picked_up, destination);

        current_cup = select_new_current(cups, current_cup);

        --moves;
    }

    return calculate_cup_order(cups);
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 23\n");

    std::string input = "589174263";

    fmt::print("Part 1 Solution: {}\n", part1(digits_to_list(input)));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>


TEST_CASE("Can split string of digits to list of ints")
{
    REQUIRE(std::list{3, 8, 9, 1, 2, 5, 4, 6, 7} == digits_to_list("389125467"));
}

TEST_CASE("Can combine list of ints to string of digits")
{
    REQUIRE(std::string{"389125467"} == list_to_digits(std::list{3, 8, 9, 1, 2, 5, 4, 6, 7}));
}

TEST_CASE("Can pick up cups")
{
    auto cups        = digits_to_list("389125467");
    auto current_cup = cups.front();
    REQUIRE(std::list{8, 9, 1} == pick_up(cups, current_cup));
    REQUIRE(std::list{3, 2, 5, 4, 6, 7} == cups);
}

TEST_CASE("Can calculate destination cup")
{
    auto cups        = digits_to_list("389125467");
    int  min_cup     = rs::min(cups);
    int  max_cup     = rs::max(cups);
    int  current_cup = cups.front();

    int destination = calculate_destination(current_cup, pick_up(cups, current_cup), min_cup, max_cup);

    REQUIRE(2 == destination);
}

TEST_CASE("Can place cups")
{
    auto cups        = digits_to_list("389125467");
    int  min_cup     = rs::min(cups);
    int  max_cup     = rs::max(cups);
    int  current_cup = cups.front();
    auto picked_up   = pick_up(cups, current_cup);
    int  destination = calculate_destination(current_cup, picked_up, min_cup, max_cup);
    move_cups(cups, picked_up, destination);

    REQUIRE(std::list{3, 2, 8, 9, 1, 5, 4, 6, 7} == cups);
}

TEST_CASE("Can select new current cup")
{
    auto cups        = digits_to_list("389125467");
    int  min_cup     = rs::min(cups);
    int  max_cup     = rs::max(cups);
    int  current_cup = cups.front();
    auto picked_up   = pick_up(cups, current_cup);
    int  destination = calculate_destination(current_cup, picked_up, min_cup, max_cup);
    move_cups(cups, picked_up, destination);

    current_cup = select_new_current(cups, current_cup);

    REQUIRE(2 == current_cup);
}

TEST_CASE("Can calculate cup order")
{
    REQUIRE(std::string{"92658374"} == calculate_cup_order(std::list{5, 8, 3, 7, 4, 1, 9, 2, 6}));
}

TEST_CASE("Can solve part 1 example")
{
    REQUIRE(std::string{"92658374"} == part1(digits_to_list("389125467"), 10));
    REQUIRE(std::string{"67384529"} == part1(digits_to_list("389125467")));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
