#include <aoc/aoc.hpp>

#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>

#include <iostream>
#include <map>
#include <optional>
#include <set>

namespace rs = ranges;
namespace rv = ranges::views;

struct tile {
    int64_t                        id;
    std::vector<std::vector<char>> image_data;
};

struct tile_compare {
    bool operator()(const tile& lhs, const tile& rhs) const { return lhs.id < rhs.id; }
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
auto flip(const std::vector<std::vector<T>>& grid)
{
    return grid | rv::transform([](auto&& s) { return s | rv::reverse | rs::to_vector; })
           | rs::to_vector;
}

std::vector<char> left_side(const tile& t)
{
    return t.image_data | rv::transform([](auto&& t) { return rs::front(t); }) | rs::to_vector;
}

std::vector<char> right_side(const tile& t)
{
    return t.image_data | rv::transform([](auto&& t) { return rs::back(t); }) | rs::to_vector;
}

bool is_top_neighbor(const tile& t1, const tile& t2)
{
    return rs::front(t1.image_data) == rs::back(t2.image_data);
}

bool is_bottom_neighbor(const tile& t1, const tile& t2)
{
    return rs::back(t1.image_data) == rs::front(t2.image_data);
}

bool is_left_neighbor(const tile& t1, const tile& t2)
{
    return left_side(t1) == right_side(t2);
}

bool is_right_neighbor(const tile& t1, const tile& t2)
{
    return right_side(t1) == left_side(t2);
}

bool is_neighbor(const tile& t1, tile& t2)
{
    for (int i : rv::iota(0, 8)) {
        // test at all sides and rotations/flip
        if (is_top_neighbor(t1, t2) || is_bottom_neighbor(t1, t2) || is_left_neighbor(t1, t2)
            || is_right_neighbor(t1, t2)) {
            return true;
        }

        t2.image_data = i % 4 == 0 ? flip(t2.image_data) : aoc::transpose(t2.image_data);
    }

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

auto build_neighbor_map(const std::vector<tile>& tiles)
{
    return tiles
           | rv::transform([&tiles](auto&& t) { return std::make_pair(t, find_neighbors(tiles, t)); })
           | rs::to<std::map<tile, std::vector<tile>, tile_compare>>;
}

std::vector<std::optional<tile>>
place_top_left(const std::map<tile, std::vector<tile>, tile_compare>& all_neighbors, int width)
{
    std::vector<std::optional<tile>> image;
    image.resize(all_neighbors.size());

    int row    = 0;
    int column = 0;

    auto first_corner = rs::find_if(all_neighbors, [](const auto& p) { return p.second.size() == 2; });

    image[(row * width) + column] = first_corner->first;

    auto neighbor = first_corner->second[0];

    bool found_top_left_right_neighbor  = false;
    bool found_top_left_bottom_neighbor = false;

    for (int i : rv::iota(0, 8)) {
        // test at all sides and rotations/flip
        if (is_right_neighbor(*image[(row * width) + column], neighbor)) {
            found_top_left_right_neighbor     = true;
            image[(row * width) + column + 1] = neighbor;
            break;
        }

        for (int j : rv::iota(0, 8)) {
            // test at all sides and rotations/flip
            if (is_right_neighbor(*image[(row * width) + column], neighbor)) {
                found_top_left_right_neighbor     = true;
                image[(row * width) + column + 1] = neighbor;
                break;
            }

            neighbor.image_data = j % 4 == 0 ? flip(neighbor.image_data)
                                             : aoc::transpose(neighbor.image_data);
        }

        if (found_top_left_right_neighbor) { break; }

        image[(row * width) + column]->image_data = i % 4 == 0
                                                        ? flip(image[(row * width) + column]->image_data)
                                                        : aoc::transpose(
                                                            image[(row * width) + column]->image_data);
    }

    neighbor = first_corner->second[1];

    for (int i = 0; i < 2; ++i) {
        for (int j : rv::iota(0, 8)) {
            // test at all sides and rotations/flip
            if (is_bottom_neighbor(*image[(row * width) + column], neighbor)) {
                found_top_left_bottom_neighbor      = true;
                image[((row + 1) * width) + column] = neighbor;
                break;
            }

            neighbor.image_data = j % 4 == 0 ? flip(neighbor.image_data)
                                             : aoc::transpose(neighbor.image_data);
        }

        if (found_top_left_bottom_neighbor) break;

        image[(row * width) + column]->image_data = flip(image[(row * width) + column]->image_data);
        image[(row * width) + column]->image_data = aoc::transpose(
            image[(row * width) + column]->image_data);
        image[(row * width) + column]->image_data = aoc::transpose(
            image[(row * width) + column]->image_data);
        image[(row * width) + column + 1]->image_data = flip(
            image[(row * width) + column + 1]->image_data);
        image[(row * width) + column + 1]->image_data = aoc::transpose(
            image[(row * width) + column + 1]->image_data);
        image[(row * width) + column + 1]->image_data = aoc::transpose(
            image[(row * width) + column + 1]->image_data);
    }

    return image;
}

std::optional<tile> find_right_neighbor(const std::vector<tile>& neighbors, const tile& t)
{
    for (auto neighbor : neighbors) {
        for (int j : rv::iota(0, 8)) {
            // test at all sides and rotations/flip
            if (is_right_neighbor(t, neighbor)) { return neighbor; }

            neighbor.image_data = j % 4 == 0 ? flip(neighbor.image_data)
                                             : aoc::transpose(neighbor.image_data);
        }
    }

    return {};
}

std::optional<tile> find_bottom_neighbor(const std::vector<tile>& neighbors, const tile& t)
{
    for (auto neighbor : neighbors) {
        for (int j : rv::iota(0, 8)) {
            // test at all sides and rotations/flip
            if (is_bottom_neighbor(t, neighbor)) { return neighbor; }

            neighbor.image_data = j % 4 == 0 ? flip(neighbor.image_data)
                                             : aoc::transpose(neighbor.image_data);
        }
    }

    return {};
}

std::vector<std::optional<tile>>
place_tiles(const std::map<tile, std::vector<tile>, tile_compare>& all_neighbors, int width)
{
    auto image_tiles = place_top_left(all_neighbors, width);

    for (int row = 0; row < width; ++row) {
        for (int column = 0; column < width; ++column) {
            // place right_side
            if (column + 1 < width && !image_tiles[(row * width) + column + 1]) {
                auto right_neighbor = find_right_neighbor(
                    all_neighbors.at(*image_tiles[(row * width) + column]),
                    *image_tiles[(row * width) + column]);
                if (right_neighbor) { image_tiles[(row * width) + column + 1] = right_neighbor.value(); }
            }
            // place bottom_side
            if (row + 1 < width && !image_tiles[((row + 1) * width) + column]) {
                auto bottom_neighbor = find_bottom_neighbor(
                    all_neighbors.at(*image_tiles[(row * width) + column]),
                    *image_tiles[(row * width) + column]);
                if (bottom_neighbor) {
                    image_tiles[((row + 1) * width) + column] = bottom_neighbor.value();
                }
            }
        }
    }

    return image_tiles;
}

tile remove_borders(tile t)
{
    t.image_data = (t.image_data | rv::tail) | rv::drop_last(1) | rv::transform([](auto&& row) {
                       return (row | rv::tail) | rv::drop_last(1) | rs::to_vector;
                   })
                   | rs::to_vector;
    return t;
}

std::vector<std::vector<char>> stitch_tiles(const std::vector<tile>& image, int width)
{
    std::vector<std::vector<char>> stitched_image;

    int tile_width = static_cast<int>(image[0].image_data[0].size());

    stitched_image.resize(width * tile_width);

    for (auto& row : stitched_image) {
        row.resize(width * tile_width);
    }

    for (int image_row = 0; image_row < width; ++image_row) {
        for (int tile_row = 0; tile_row < tile_width; ++tile_row) {
            for (int image_column = 0; image_column < width; ++image_column) {
                for (int tile_column = 0; tile_column < tile_width; ++tile_column) {
                    const auto& tile = image[(image_row * width) + image_column];

                    stitched_image[image_row * tile_width + tile_row]
                                  [image_column * tile_width + tile_column]
                        = tile.image_data[tile_row][tile_column];
                }
            }
        }
    }

    return stitched_image;
}

int count_monsters(std::vector<std::vector<char>> image)
{
    int monster_count = 0;

    for (auto rng : image | rv::sliding(3)) {
        auto slice = rng | rs::to_vector;
        for (int i = 0; i < slice[0].size() - 20; ++i) {
            if (slice[1][i] == '#' && slice[2][i + 1] == '#' && slice[2][i + 4] == '#'
                && slice[1][i + 5] == '#' && slice[1][i + 6] == '#' && slice[2][i + 7] == '#'
                && slice[2][i + 10] == '#' && slice[1][i + 11] == '#' && slice[1][i + 12] == '#'
                && slice[2][i + 13] == '#' && slice[2][i + 16] == '#' && slice[1][i + 17] == '#'
                && slice[1][i + 18] == '#' && slice[0][i + 18] == '#' && slice[1][i + 19] == '#') {
                ++monster_count;
            }
        }
    }

    return monster_count;
}

int64_t part1(std::map<tile, std::vector<tile>, tile_compare> neighbor_map)
{
    return rs::accumulate(
        neighbor_map | rv::filter([](const auto& p) { return p.second.size() == 2; })
            | rv::transform([](auto&& p) { return p.first.id; }),
        int64_t{1},
        std::multiplies<>{});
}

int64_t part2(std::map<tile, std::vector<tile>, tile_compare> neighbor_map, int width)
{
    auto image_tiles = place_tiles(neighbor_map, width);

    auto trimmed_tiles = image_tiles | rv::transform([](auto&& t) { return remove_borders(*t); })
                         | rs::to_vector;

    auto image = stitch_tiles(trimmed_tiles, width);

    int monsters_found    = count_monsters(image);
    int tiles_per_monster = 15;

    int i = 0;
    while (monsters_found == 0 && i < 8) {
        image          = i++ % 4 == 0 ? flip(image) : aoc::transpose(image);
        monsters_found = count_monsters(image);
    }

    auto water_roughness = rs::count(image | rv::join, '#');

    return water_roughness - (monsters_found * tiles_per_monster);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 20\n");

    auto input        = read_input(std::ifstream{"puzzle.in"});
    auto neighbor_map = build_neighbor_map(input);
    auto width        = static_cast<int>(std::sqrt(neighbor_map.size()));

    fmt::print("Part 1 Solution: {}\n", part1(neighbor_map));
    fmt::print("Part 2 Solution: {}\n", part2(neighbor_map, width));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can count monsters")
{
    // clang-format off
    auto image = std::vector{
        std::vector{'.', '#', '.', '#', '.', '.', '#', '.', '#', '#', '.', '.', '.', '#', '.', '#', '#', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '.', '.', '.', '.', '#', '.', '#', '.', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '.', '.', '.'},
        std::vector{'#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '.', '#', '.', '.', '#', '#', '#', '#', '#', '#', '.', '.', '.'},
        std::vector{'#', '#', '#', '.', '#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '#', '#', '#', '#', '#', '.', '#', '.', '.', '#'},
        std::vector{'#', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '.', '.', '#', '.', '#', '#'},
        std::vector{'.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.', '.', '.', '.', '#', '#', '#', '#', '#', '.', '#'},
        std::vector{'.', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '#', '.', '.', '#', '.', '#', '.', '#', '#', '#', '.', '.'},
        std::vector{'.', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.'},
        std::vector{'#', '.', '.', '#', '.', '#', '#', '.', '.', '#', '.', '.', '#', '#', '#', '.', '#', '.', '#', '#', '.', '.', '.', '.'},
        std::vector{'#', '.', '#', '#', '#', '#', '.', '.', '#', '.', '#', '#', '#', '#', '.', '#', '.', '#', '.', '#', '#', '#', '.', '.'},
        std::vector{'#', '#', '#', '.', '#', '.', '#', '.', '.', '.', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '.', '.', '#', '#'},
        std::vector{'#', '.', '#', '#', '#', '#', '.', '.', '.', '.', '#', '#', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#'},
        std::vector{'#', '#', '.', '.', '#', '#', '.', '#', '.', '.', '.', '#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '#', '.', '.'},
        std::vector{'.', '.', '.', '#', '.', '.', '#', '.', '.', '#', '.', '#', '.', '#', '#', '.', '.', '#', '#', '#', '.', '#', '#', '#'},
        std::vector{'.', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#', '#', '.', '#', '.', '.', '.', '#', '#', '#', '.', '#', '#', '.'},
        std::vector{'#', '#', '#', '.', '#', '.', '.', '.', '#', '.', '.', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '.'},
        std::vector{'.', '#', '.', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '.', '.', '#', '.', '#', '#', '.', '.'},
        std::vector{'.', '#', '#', '#', '#', '.', '#', '#', '#', '.', '#', '.', '.', '.', '#', '#', '#', '.', '#', '.', '.', '#', '.', '#'},
        std::vector{'.', '.', '#', '.', '#', '.', '.', '#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#'},
        std::vector{'#', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '.', '.', '.', '.', '#', '#'},
        std::vector{'#', '.', '#', '#', '.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#'},
        std::vector{'.', '#', '.', '#', '#', '#', '.', '.', '#', '#', '.', '.', '#', '#', '.', '.', '#', '#', '#', '#', '.', '#', '#', '.'},
        std::vector{'.', '.', '.', '#', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '#', '.', '.', '.', '#', '.', '.', '#', '#', '#'}};
    // clang-format on

    image = aoc::transpose(image);
    image = flip(image);

    REQUIRE(2 == count_monsters(image));
}

TEST_CASE("Can stitch images together")
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

    auto input         = read_input(std::move(ss));
    auto neighbor_map  = build_neighbor_map(input);
    auto image_tiles   = place_tiles(neighbor_map, 3);
    auto trimmed_tiles = image_tiles | rv::transform([](auto&& t) { return remove_borders(*t); })
                         | rs::to_vector;

    // clang-format off
    auto expected = std::vector{
        std::vector{'.', '#', '.', '#', '.', '.', '#', '.', '#', '#', '.', '.', '.', '#', '.', '#', '#', '.', '.', '#', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '.', '.', '.', '.', '#', '.', '#', '.', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '.', '.', '.'},
        std::vector{'#', '#', '.', '#', '#', '.', '#', '#', '#', '.', '#', '.', '#', '.', '.', '#', '#', '#', '#', '#', '#', '.', '.', '.'},
        std::vector{'#', '#', '#', '.', '#', '#', '#', '#', '#', '.', '.', '.', '#', '.', '#', '#', '#', '#', '#', '.', '#', '.', '.', '#'},
        std::vector{'#', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#', '#', '.', '#', '#', '#', '#', '.', '.', '.', '#', '.', '#', '#'},
        std::vector{'.', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#', '.', '.', '.', '.', '#', '#', '#', '#', '#', '.', '#'},
        std::vector{'.', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '#', '.', '.', '#', '.', '#', '.', '#', '#', '#', '.', '.'},
        std::vector{'.', '#', '#', '#', '#', '.', '.', '.', '#', '.', '.', '#', '.', '.', '.', '.', '.', '#', '.', '.', '.', '.', '.', '.'},
        std::vector{'#', '.', '.', '#', '.', '#', '#', '.', '.', '#', '.', '.', '#', '#', '#', '.', '#', '.', '#', '#', '.', '.', '.', '.'},
        std::vector{'#', '.', '#', '#', '#', '#', '.', '.', '#', '.', '#', '#', '#', '#', '.', '#', '.', '#', '.', '#', '#', '#', '.', '.'},
        std::vector{'#', '#', '#', '.', '#', '.', '#', '.', '.', '.', '#', '.', '#', '#', '#', '#', '#', '#', '.', '#', '.', '.', '#', '#'},
        std::vector{'#', '.', '#', '#', '#', '#', '.', '.', '.', '.', '#', '#', '.', '.', '#', '#', '#', '#', '#', '#', '#', '#', '.', '#'},
        std::vector{'#', '#', '.', '.', '#', '#', '.', '#', '.', '.', '.', '#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '#', '.', '.'},
        std::vector{'.', '.', '.', '#', '.', '.', '#', '.', '.', '#', '.', '#', '.', '#', '#', '.', '.', '#', '#', '#', '.', '#', '#', '#'},
        std::vector{'.', '#', '.', '#', '.', '.', '.', '.', '#', '.', '#', '#', '.', '#', '.', '.', '.', '#', '#', '#', '.', '#', '#', '.'},
        std::vector{'#', '#', '#', '.', '#', '.', '.', '.', '#', '.', '.', '#', '.', '#', '#', '.', '#', '#', '#', '#', '#', '#', '.', '.'},
        std::vector{'.', '#', '.', '#', '.', '#', '#', '#', '.', '#', '#', '.', '#', '#', '.', '#', '.', '.', '#', '.', '#', '#', '.', '.'},
        std::vector{'.', '#', '#', '#', '#', '.', '#', '#', '#', '.', '#', '.', '.', '.', '#', '#', '#', '.', '#', '.', '.', '#', '.', '#'},
        std::vector{'.', '.', '#', '.', '#', '.', '.', '#', '.', '.', '#', '.', '#', '.', '#', '.', '#', '#', '#', '#', '.', '#', '#', '#'},
        std::vector{'#', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#', '.', '#', '.', '#', '.', '#', '#', '#', '.', '#', '#', '#', '.'},
        std::vector{'#', '#', '#', '#', '#', '.', '.', '#', '#', '#', '#', '#', '.', '.', '.', '#', '#', '#', '.', '.', '.', '.', '#', '#'},
        std::vector{'#', '.', '#', '#', '.', '.', '#', '.', '.', '#', '.', '.', '.', '#', '.', '.', '#', '#', '#', '#', '.', '.', '.', '#'},
        std::vector{'.', '#', '.', '#', '#', '#', '.', '.', '#', '#', '.', '.', '#', '#', '.', '.', '#', '#', '#', '#', '.', '#', '#', '.'},
        std::vector{'.', '.', '.', '#', '#', '#', '.', '.', '.', '#', '#', '.', '.', '.', '#', '.', '.', '.', '#', '.', '.', '#', '#', '#'}};

    expected = aoc::transpose(expected);
    expected = aoc::transpose(expected);

    // clang-format on

    REQUIRE(expected == stitch_tiles(trimmed_tiles, 3));
}

TEST_CASE("Can remove tile borders")
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

    t = remove_borders(t);

    auto expected = std::vector{
        std::vector{'.', '.', '.', '.', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '.', '.', '.', '.'},
        std::vector{'.', '.', '.', '.', '#', '#', '#', '#'},
        std::vector{'#', '#', '#', '#', '.', '.', '.', '.'}};


    REQUIRE(expected == t.image_data);
}

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

    REQUIRE(expected == aoc::transpose(t.image_data));
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

    auto input        = read_input(std::move(ss));
    auto neighbor_map = build_neighbor_map(input);

    REQUIRE(20899048083289 == part1(neighbor_map));
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

    auto input        = read_input(std::move(ss));
    auto neighbor_map = build_neighbor_map(input);

    REQUIRE(273 == part2(neighbor_map, 3));
}

#endif
