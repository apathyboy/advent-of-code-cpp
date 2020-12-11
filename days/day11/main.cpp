#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <optional>

namespace ra = ranges::actions;
namespace rv = ranges::views;

enum class direction {
    TOP_LEFT = 0,
    TOP_MIDDLE,
    TOP_RIGHT,
    LEFT,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM_MIDDLE,
    BOTTOM_RIGHT
};

const std::vector<direction> ALL_DIRECTIONS = {
    direction::TOP_LEFT,
    direction::TOP_MIDDLE,
    direction::TOP_RIGHT,
    direction::LEFT,
    direction::RIGHT,
    direction::BOTTOM_LEFT,
    direction::BOTTOM_MIDDLE,
    direction::BOTTOM_RIGHT};

std::pair<std::vector<char>, int64_t> read_input(std::istream&& input)
{
    auto lines = ranges::getlines(input) | ranges::to<std::vector>;

    return std::make_pair(lines | rv::join | ranges::to<std::vector>, lines[0].length());
}

std::optional<int64_t>
calculate_adjacent_index(int64_t map_size, int64_t stride, int64_t idx, direction d)
{
    int64_t                spot_on_line = idx % stride;
    std::optional<int64_t> adjacent_idx;

    switch (d) {
        case direction::TOP_LEFT: {
            if (spot_on_line > 0)
                adjacent_idx = idx - stride - 1;
        } break;
        case direction::TOP_MIDDLE: {
            adjacent_idx = idx - stride;
        } break;
        case direction::TOP_RIGHT: {
            if (spot_on_line < stride - 1)
                adjacent_idx = idx - stride + 1;
        } break;
        case direction::LEFT: {
            if (spot_on_line > 0)
                adjacent_idx = idx - 1;
        } break;
        case direction::RIGHT: {
            if (spot_on_line < stride - 1)
                adjacent_idx = idx + 1;
        } break;
        case direction::BOTTOM_LEFT: {
            if (spot_on_line > 0)
                adjacent_idx = idx + stride - 1;
        } break;
        case direction::BOTTOM_MIDDLE: {
            adjacent_idx = idx + stride;
        } break;
        case direction::BOTTOM_RIGHT: {
            if (spot_on_line < stride - 1)
                adjacent_idx = idx + stride + 1;
        } break;
    }

    if (adjacent_idx && (adjacent_idx.value() < 0 || adjacent_idx.value() >= map_size))
        adjacent_idx = std::nullopt;


    return adjacent_idx;
}

std::vector<int64_t> find_adjacent_indexes(int64_t map_size, int64_t stride, int64_t idx)
{
    auto adjacent_indexes = ALL_DIRECTIONS | rv::transform([map_size, stride, idx](auto dir) {
                                return calculate_adjacent_index(map_size, stride, idx, dir);
                            })
                            | rv::filter([](const auto& idx) { return idx.has_value(); })
                            | rv::transform([](const auto& idx) { return idx.value(); })
                            | ranges::to<std::vector>;

    return adjacent_indexes;
}

int64_t count_occupied_adjacent(const std::vector<char>& input, int64_t stride, int64_t idx)
{
    auto adjacent_indexes = find_adjacent_indexes(input.size(), stride, idx);

    return ranges::distance(
        adjacent_indexes | rv::transform([&input](auto idx) { return input[idx] == '#'; })
        | rv::filter([](auto b) { return b; }));
}

bool is_visibly_occupied(
    const std::vector<char>& input,
    int64_t                  stride,
    int64_t                  idx,
    direction                d)
{
    auto adjacent_idx = calculate_adjacent_index(input.size(), stride, idx, d);

    if (!adjacent_idx || input[adjacent_idx.value()] == 'L')
        return false;
    if (adjacent_idx && input[adjacent_idx.value()] == '#')
        return true;
    else
        return is_visibly_occupied(input, stride, adjacent_idx.value(), d);
}

int count_occupied_visible_adjacent(
    const std::vector<char>& input,
    int64_t                  stride,
    int64_t                  idx)
{
    return ranges::accumulate(
        ALL_DIRECTIONS | rv::transform([&input, stride, idx](auto dir) {
            return is_visibly_occupied(input, stride, idx, dir) ? 1 : 0;
        }),
        0);
}

auto part1_map_rule(const std::vector<char>& input, int64_t stride)
{
    return [&input, stride](auto&& rng) {
        auto [idx, c] = rng;

        if (c != '.') {
            auto occupied_adjacents = count_occupied_adjacent(input, stride, idx);

            if (c == 'L' && occupied_adjacents == 0) {
                c = '#';
            }
            else if (c == '#' && occupied_adjacents >= 4) {
                c = 'L';
            }
        }

        return c;
    };
}

auto part2_map_rule(const std::vector<char>& input, int64_t stride)
{
    return [&input, stride](auto&& rng) {
        auto [idx, c] = rng;

        if (c != '.') {
            auto occupied_adjacents = count_occupied_visible_adjacent(input, stride, idx);

            if (c == 'L' && occupied_adjacents == 0) {
                c = '#';
            }
            else if (c == '#' && occupied_adjacents >= 5) {
                c = 'L';
            }
        }

        return c;
    };
}

int64_t part1(std::vector<char> input, int64_t stride)
{
    int64_t last_counter, current_counter = ranges::count(input, '#');

    do {
        last_counter = current_counter;

        auto tmp = input;

        input = rv::enumerate(input) | rv::transform(part1_map_rule(tmp, stride))
                | ranges::to<std::vector>;

        current_counter = ranges::count(input, '#');
    } while (last_counter != current_counter);

    return current_counter;
}

int64_t part2(std::vector<char> input, int64_t stride)
{
    int64_t last_counter, current_counter = ranges::count(input, '#');

    do {
        last_counter = current_counter;

        auto tmp = input;

        input = rv::enumerate(input) | rv::transform(part2_map_rule(tmp, stride))
                | ranges::to<std::vector>;

        current_counter = ranges::count(input, '#');
    } while (last_counter != current_counter);

    return current_counter;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 11\n");

    auto [input, stride] = read_input(std::ifstream{"days/day11/input.txt"});

    fmt::print("Part 1 Solution: {}\n", part1(input, stride));
    fmt::print("Part 2 Solution: {}\n", part2(input, stride));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>


TEST_CASE("Can solve day 11 problems")
{
    std::stringstream ss;

    ss << R"(L.LL.LL.LL
LLLLLLL.LL
L.L.L..L..
LLLL.LL.LL
L.LL.LL.LL
L.LLLLL.LL
..L.L.....
LLLLLLLLLL
L.LLLLLL.L
L.LLLLL.LL)";

    auto [input, stride] = read_input(std::move(ss));

    SECTION("Can solve part 1 example") { REQUIRE(37 == part1(input, stride)); }

    SECTION("Can solve part 2 example") { REQUIRE(26 == part2(input, stride)); }
}

#endif
