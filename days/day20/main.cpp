#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>

#include <iostream>
#include <map>
#include <set>

namespace rs = ranges;
namespace rv = ranges::views;

struct tile {
    int64_t                        id;
    std::vector<std::vector<char>> image_data;
};

int parse_tile_id(const std::string& s)
{
    return std::stoi(s.substr(5, s.length() - 1));
}

std::vector<tile> read_input(std::istream&& input)
{
    return rs::getlines(input) | rv::split("") | rv::transform([](auto&& rng) {
               auto v = rng | rv::transform([](auto&& s) { return s | rs::to<std::string>; })
                        | rs::to_vector;

               tile t{parse_tile_id(v[0]), rv::tail(v) | rs::to<std::vector<std::vector<char>>>};

               return t;
           })
           | rs::to_vector;
}


template <typename T>
auto rotate_tile(const std::vector<std::vector<T>>& grid)
{
    return rv::iota(0, static_cast<int>(grid[0].size())) | rv::transform([&grid](int i) {
               return grid | rv::transform([i](auto&& t) { return t[i]; }) | rv::reverse | rs::to_vector;
           })
           | rs::to_vector;
}

template <typename T>
auto flip(const std::vector<std::vector<T>>& grid)
{
    return grid | rv::transform([](auto&& s) { return s | rv::reverse | rs::to_vector; })
           | rs::to_vector;
}

std::vector<char> left_side(const tile& t)
{
    return t.image_data | rv::transform([](auto&& t) { return t[0]; }) | rs::to_vector;
}

std::vector<char> right_side(const tile& t)
{
    int pos = static_cast<int>(t.image_data[0].size() - 1);
    return t.image_data | rv::transform([pos](auto&& t) { return t[pos]; }) | rs::to_vector;
}

/*
bool set_neighbor(tile& t, tile& t2)
{
    // compare top
    if (rs::front(t.image_data) == rs::back(t2.image_data)) {
        if (!t.bottom.contains(&t2)) {
            t.top.insert(&t2);
            t2.bottom.insert(&t);
        }
        return true;
    }
    // compare bottom
    if (rs::back(t.image_data) == rs::front(t2.image_data)) {
        if (!t.top.contains(&t2)) {
            t.bottom.insert(&t2);
            t2.top.insert(&t);
        }
        return true;
    }

    auto t_left  = left_side(t);
    auto t_right = right_side(t);

    auto t2_left  = left_side(t2);
    auto t2_right = right_side(t2);

    // compare left
    if (t_left == t2_right) {
        if (!t.right.contains(&t2)) {
            t.left.insert(&t2);
            t2.right.insert(&t);
        }
        return true;
    }
    // compare right
    if (t_right == t2_left) {
        if (!t.left.contains(&t2)) {
            t.right.insert(&t2);
            t2.left.insert(&t);
        }
        return true;
    }

    return false;
}

void print_tile(const tile& t)
{
    for (auto row : t.image_data) {
        fmt::print("{}\n", fmt::join(row, ""));
    }
}

void set_neighbors(std::vector<tile>& tiles, tile& t)
{
    for (auto& check_tile : tiles) {
        if (t.id == check_tile.id) continue;

        if (set_neighbor(t, check_tile)) { continue; }

        // if (check_tile.morphed) continue;
        check_tile.morphed = true;

        check_tile.image_data = rotate_tile(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.image_data = rotate_tile(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.image_data = rotate_tile(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.image_data = flip(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.image_data = rotate_tile(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.image_data = rotate_tile(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.image_data = rotate_tile(check_tile.image_data);

        if (set_neighbor(t, check_tile)) { continue; }

        check_tile.morphed = false;
    }
}

const tile& find_topleft(const std::vector<tile>& tiles)
{
    auto find_iter = rs::find_if(tiles, [](const auto& t) {
        return t.right.size() > 0 && t.bottom.size() > 0 && t.top.size() == 0 && t.left.size() == 0;
    });

    return *find_iter;
}

const tile& find_bottomleft(const std::vector<tile>& tiles)
{
    auto find_iter = rs::find_if(tiles, [](const auto& t) {
        return t.right.size() > 0 && t.bottom.size() == 0 && t.top.size() > 0 && t.left.size() == 0;
    });

    return *find_iter;
}

const tile& find_topright(const std::vector<tile>& tiles)
{
    auto find_iter = rs::find_if(tiles, [](const auto& t) {
        return t.right.size() == 0 && t.bottom.size() > 0 && t.top.size() == 0 && t.left.size() > 0;
    });

    return *find_iter;
}

const tile& find_bottomright(const std::vector<tile>& tiles)
{
    auto find_iter = rs::find_if(tiles, [](const auto& t) {
        return t.right.size() == 0 && t.bottom.size() == 0 && t.top.size() > 0 && t.left.size() > 0;
    });

    return *find_iter;
}

std::vector<tile*> find_corners(std::vector<tile>& tiles)
{
    auto tmp = tiles;
    for (auto& tile : tiles) {
        set_neighbors(tmp, tile);

        tmp = tiles;
    }

    return tiles | rv::filter([](const auto& t) {
               return (t.top.size() > 0 ? 1 : 0) + (t.bottom.size() > 0 ? 1 : 0)
                          + (t.left.size() > 0 ? 1 : 0) + (t.right.size() > 0 ? 1 : 0)
                      == 2;
           })
           | rv::transform([](auto& t) { return &t; }) | rs::to_vector;
}

int64_t part1(std::vector<tile> tiles)
{
    auto corners = find_corners(tiles);

    auto corner_ids = corners | rv::transform([](auto&& t) { return t->id; });

    std::cout << corner_ids << std::endl;

    return rs::accumulate(corner_ids, int64_t{1}, std::multiplies<>{});
}
*/

bool is_neighbor(const tile& t1, tile& t2)
{
    // test at all sides and rotations/flip
    (t1, t2);
    return false;
}

std::vector<tile> find_neighbors(std::vector<tile> tiles, const tile& t)
{
    std::vector<tile> neighbors;
    for (auto check_tile : tiles) {
        if (t.id == check_tile.id) continue;
        if (is_neighbor(t, check_tile)) { neighbors.push_back(check_tile); }
    }
    return neighbors;
}

int64_t part1(std::vector<tile> tiles)
{
    std::map<int64_t, std::vector<tile>> neighbor_map;

    for (auto& t : tiles) {
        neighbor_map.insert(std::make_pair(t.id, find_neighbors(tiles, t)));
    }

    return rs::accumulate(
        neighbor_map | rv::filter([](const auto& p) { return p.second.size() == 2; })
            | rv::transform([](auto&& p) { return p.first; }),
        int64_t{1},
        std::multiplies<>{});
}

int64_t part2(std::vector<tile> tiles)
{
    (tiles);
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 20\n");

    std::string input_path = "days/day20/example.in";

    auto input = read_input(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can rotate tile")
{
    tile t;
    t.id         = 0;
    t.image_data = std::vector{
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'}};

    auto expected = std::vector{
        std::vector{'.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        std::vector{'.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        std::vector{'.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        std::vector{'.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        std::vector{'.', '#', '.', '#', '.', '#', '.', '#', '.', '#'},
        std::vector{'#', '.', '#', '.', '#', '.', '#', '.', '#', '.'},
        std::vector{'#', '.', '#', '.', '#', '.', '#', '.', '#', '.'},
        std::vector{'#', '.', '#', '.', '#', '.', '#', '.', '#', '.'},
        std::vector{'#', '.', '#', '.', '#', '.', '#', '.', '#', '.'},
        std::vector{'#', '.', '#', '.', '#', '.', '#', '.', '#', '.'}};

    REQUIRE(expected == rotate_tile(t.image_data));
}

TEST_CASE("Can flip tile")
{
    tile t;
    t.id         = 0;
    t.image_data = std::vector{
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'}};

    auto expected = std::vector{
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '.', '.', '.'}};

    REQUIRE(expected == flip(t.image_data));
}

TEST_CASE("Can get left side of tile")
{
    tile t;
    t.id         = 0;
    t.image_data = std::vector{
        std::vector{'.', '.', '#', '#', '.', '#', '.', '.', '#', '.'},
        std::vector{'#', '#', '.', '.', '#', '.', '.', '.', '.', '.'},
        std::vector{'#', '.', '.', '.', '#', '#', '.', '.', '#', '.'},
        std::vector{'#', '#', '#', '#', '.', '#', '.', '.', '.', '#'},
        std::vector{'#', '#', '.', '#', '#', '.', '#', '#', '#', '.'},
        std::vector{'#', '#', '.', '.', '.', '#', '.', '#', '#', '#'},
        std::vector{'.', '#', '.', '#', '.', '#', '.', '.', '#', '#'},
        std::vector{'.', '.', '#', '.', '.', '.', '.', '#', '.', '.'},
        std::vector{'#', '#', '#', '.', '.', '.', '#', '.', '#', '.'},
        std::vector{'.', '.', '#', '#', '#', '.', '.', '#', '#', '#'}};

    std::vector expected{'.', '#', '#', '#', '#', '#', '.', '.', '#', '.'};

    REQUIRE(expected == left_side(t));
}

TEST_CASE("Can get right side of tile")
{
    tile t;
    t.id         = 0;
    t.image_data = std::vector{
        std::vector{'.', '.', '#', '#', '.', '#', '.', '.', '#', '.'},
        std::vector{'#', '#', '.', '.', '#', '.', '.', '.', '.', '.'},
        std::vector{'#', '.', '.', '.', '#', '#', '.', '.', '#', '.'},
        std::vector{'#', '#', '#', '#', '.', '#', '.', '.', '.', '#'},
        std::vector{'#', '#', '.', '#', '#', '.', '#', '#', '#', '.'},
        std::vector{'#', '#', '.', '.', '.', '#', '.', '#', '#', '#'},
        std::vector{'.', '#', '.', '#', '.', '#', '.', '.', '#', '#'},
        std::vector{'.', '.', '#', '.', '.', '.', '.', '#', '.', '.'},
        std::vector{'#', '#', '#', '.', '.', '.', '#', '.', '#', '.'},
        std::vector{'.', '.', '#', '#', '#', '.', '.', '#', '#', '#'}};

    std::vector expected{'.', '.', '.', '#', '.', '#', '#', '.', '.', '#'};

    REQUIRE(expected == right_side(t));
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...)";

    auto input = read_input(std::move(ss));

    REQUIRE(20899048083289 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(Tile 2311:
..##.#..#.
##..#.....
#...##..#.
####.#...#
##.##.###.
##...#.###
.#.#.#..##
..#....#..
###...#.#.
..###..###

Tile 1951:
#.##...##.
#.####...#
.....#..##
#...######
.##.#....#
.###.#####
###.##.##.
.###....#.
..#.#..#.#
#...##.#..

Tile 1171:
####...##.
#..##.#..#
##.#..#.#.
.###.####.
..###.####
.##....##.
.#...####.
#.##.####.
####..#...
.....##...

Tile 1427:
###.##.#..
.#..#.##..
.#.##.#..#
#.#.#.##.#
....#...##
...##..##.
...#.#####
.#.####.#.
..#..###.#
..##.#..#.

Tile 1489:
##.#.#....
..##...#..
.##..##...
..#...#...
#####...#.
#..#.#.#.#
...#.#.#..
##.#...##.
..##.##.##
###.##.#..

Tile 2473:
#....####.
#..#.##...
#.##..#...
######.#.#
.#...#.#.#
.#########
.###.#..#.
########.#
##...##.#.
..###.#.#.

Tile 2971:
..#.#....#
#...###...
#.#.###...
##.##..#..
.#####..##
.#..####.#
#..#.#..#.
..####.###
..#.#.###.
...#.#.#.#

Tile 2729:
...#.#.#.#
####.#....
..#.#.....
....#..#.#
.##..##.#.
.#.####...
####.#.#..
##.####...
##..#.##..
#.##...##.

Tile 3079:
#.#.#####.
.#..######
..#.......
######....
####.#..#.
.#...#.##.
#.#####.##
..#.###...
..#.......
..#.###...)";

    auto input = read_input(std::move(ss));

    REQUIRE(0 == part2(input));
}

#endif
