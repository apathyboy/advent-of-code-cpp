#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <optional>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

const inline std::unordered_map<char, std::size_t> COST = {{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};

struct Room {
    Room(char who, std::initializer_list<char> data);

    bool        is_finished() const;
    bool        can_accept() const;
    bool        has_visitors() const;
    char        top() const;
    std::size_t pop();
    std::size_t push(char who);
    char        at(std::size_t index) const { return data_[index]; }

private:
    char              owner_;
    std::vector<char> data_;
    std::size_t       size_;
    std::size_t       fixed_;
};

struct Puzzle {
    std::vector<char> hallway;
    std::vector<Room> rooms;

    size_t                greedy_move_to_room();
    size_t                greedy_room_to_room();
    std::optional<size_t> hallway_move(size_t home, size_t pos);
    size_t                occupied_spaces_between(size_t from, size_t to) const;
    bool                  solved() const;

    friend std::ostream& operator<<(std::ostream&, const Puzzle&);
};


Room::Room(char who, std::initializer_list<char> data)
    : owner_(who)
    , data_(data)
    , size_(data.size())
    , fixed_(0)
{
    size_ -= std::ranges::count(data_, '.');
    auto it = std::ranges::find_if_not(data_, [who](char c) { return c == who; });
    fixed_  = it - data_.begin();
}

bool Room::is_finished() const
{
    return fixed_ == data_.size();
}
bool Room::can_accept() const
{
    return fixed_ == size_ && size_ != data_.size();
}
bool Room::has_visitors() const
{
    return fixed_ != data_.size() && fixed_ != size_ && size_ != 0;
}
char Room::top() const
{
    if (size_ == 0) throw std::runtime_error("Room is empty.");
    return data_[size_ - 1];
}

size_t Room::pop()
{
    size_t cost      = (data_.size() - size_ + 1) * COST.at(data_[size_ - 1]);
    data_[size_ - 1] = '.';
    size_--;
    return cost;
}

size_t Room::push(char who)
{
    if (!can_accept()) throw std::runtime_error("Room can't accept data.");
    if (who != owner_) throw std::runtime_error("Tried to push the wrong type.");
    size_t cost  = (data_.size() - size_) * COST.at(who);
    data_[size_] = who;
    size_++;
    fixed_++;
    return cost;
}

size_t distance(size_t from, size_t to)
{
    if (to > from) return to - from;
    else
        return from - to;
}

size_t Puzzle::greedy_move_to_room()
{
    for (size_t i = 0; i < hallway.size(); i++) {
        char who = hallway[i];
        if (who == '.') continue;
        if (!rooms[who - 'A'].can_accept()) continue;
        size_t target = 2 + 2 * (who - 'A');
        if (occupied_spaces_between(i, target) > 1) continue;
        size_t cost = rooms[who - 'A'].push(who);
        hallway[i]  = '.';
        return cost + COST.at(who) * distance(i, target);
    }
    return 0;
}

size_t Puzzle::greedy_room_to_room()
{
    for (size_t h = 0; h < rooms.size(); h++) {
        if (!rooms[h].has_visitors()) continue;
        char who = rooms[h].top();
        if (!rooms[who - 'A'].can_accept()) continue;
        if (occupied_spaces_between(2 * h + 2, 2 * (who - 'A') + 2) != 0) continue;
        size_t cost = rooms[h].pop();
        cost += rooms[who - 'A'].push(who);
        cost += distance(h, who - 'A') * 2 * COST.at(who);
        return cost;
    }
    return 0;
}

size_t Puzzle::occupied_spaces_between(size_t from, size_t to) const
{
    auto b = hallway.begin() + std::min(from, to);
    auto e = hallway.begin() + std::max(from, to) + 1;
    return std::ranges::count_if(b, e, [](auto c) { return c != '.'; });
}

bool Puzzle::solved() const
{
    for (auto& room : rooms)
        if (!room.is_finished()) return false;
    return true;
}

std::optional<size_t> Puzzle::hallway_move(size_t room, size_t target)
{
    // check the room
    if (!rooms[room].has_visitors()) return {};
    // don't land in front of rooms
    if (target % 2 == 0 && target != 0 && target != 10) return {};
    size_t source = room * 2 + 2;
    if (occupied_spaces_between(source, target) != 0) return {};

    char   who  = rooms[room].top();
    size_t cost = distance(source, target) * COST.at(who);
    cost += rooms[room].pop();
    hallway[target] = who;

    return cost;
}

std::optional<size_t> find_shortest(Puzzle puzzle)
{
    size_t cost = 0;
    while (true) {
        size_t greedy = puzzle.greedy_move_to_room();
        greedy += puzzle.greedy_room_to_room();
        if (greedy == 0) break;
        cost += greedy;
    }

    if (puzzle.solved()) { return cost; }

    size_t best = std::numeric_limits<size_t>::max();
    for (size_t h = 0; h < puzzle.rooms.size(); h++) {
        if (!puzzle.rooms[h].has_visitors()) continue;
        for (size_t i = 0; i < puzzle.hallway.size(); i++) {
            if (i % 2 == 0 && i != 0 && i != 10) continue;
            if (puzzle.occupied_spaces_between(h * 2 + 2, i) != 0) continue;
            Puzzle copy      = puzzle;
            auto   move_cost = copy.hallway_move(h, i);
            if (!move_cost.has_value()) continue;
            auto recursion = find_shortest(std::move(copy));
            if (!recursion.has_value()) continue;
            best = std::min(best, move_cost.value() + recursion.value());
        }
    }
    if (best == std::numeric_limits<size_t>::max()) return {};

    return best + cost;
}

std::ostream& operator<<(std::ostream& s, const Puzzle& puzzle)
{
    for (size_t i = 0; i < puzzle.hallway.size() + 2; i++)
        s << "#";
    s << "\n";

    s << "#";
    for (size_t i = 0; i < puzzle.hallway.size(); i++)
        s << puzzle.hallway[i];
    s << "#\n";

    s << "###";
    for (size_t i = 0; i < puzzle.rooms.size(); i++)
        s << puzzle.rooms[i].at(1) << "#";
    s << "##\n";

    s << "  #";
    for (size_t i = 0; i < puzzle.rooms.size(); i++)
        s << puzzle.rooms[i].at(0) << "#";
    s << "\n";

    s << "  ";
    for (size_t i = 0; i < puzzle.hallway.size() - 2; i++)
        s << "#";
    s << "\n";

    return s;
}

size_t part1(Puzzle input)
{
    return find_shortest(input).value();
}

size_t part2(Puzzle input)
{
    return find_shortest(input).value();
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 23\n");

    std::ifstream ifs{"puzzle.in"};
    Puzzle        in1{
               .hallway = {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
               .rooms   = {{'A', {'B', 'D'}}, {'B', {'A', 'D'}}, {'C', {'B', 'C'}}, {'D', {'A', 'C'}}}};

    fmt::print("Part 1 Solution: {}\n", part1(in1));

    Puzzle in2{
        .hallway = {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        .rooms   = {
            {'A', {'B', 'D', 'D', 'D'}},
            {'B', {'A', 'B', 'C', 'D'}},
            {'C', {'B', 'A', 'B', 'C'}},
            {'D', {'A', 'C', 'A', 'C'}}}};

    fmt::print("Part 2 Solution: {}\n", part2(in2));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    Puzzle in1{
        .hallway = {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        .rooms   = {{'A', {'A', 'B'}}, {'B', {'D', 'C'}}, {'C', {'C', 'B'}}, {'D', {'A', 'D'}}}};

    REQUIRE(12521 == part1(in1));
}

TEST_CASE("Can solve part 2 example")
{
    Puzzle in2{
        .hallway = {'.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.'},
        .rooms   = {
            {'A', {'A', 'D', 'D', 'B'}},
            {'B', {'D', 'B', 'C', 'C'}},
            {'C', {'C', 'A', 'B', 'B'}},
            {'D', {'A', 'C', 'A', 'D'}}}};

    REQUIRE(44169 == part2(in2));
}

#endif
