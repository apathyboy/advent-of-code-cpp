#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace ra = ranges::actions;
namespace rv = ranges::views;

std::pair<std::vector<char>, int64_t> read_input(std::istream&& input)
{
    auto lines = ranges::getlines(input) | ranges::to<std::vector>;

    return std::make_pair(lines | rv::join | ranges::to<std::vector>, lines[0].length());
}

bool is_occupied(const std::vector<char>& input, int64_t idx)
{

    if (idx < 0 || idx > static_cast<int64_t>(input.size() - 1))
        return false;

    return input[idx] == '#';
}

int count_occupied_adjacent(const std::vector<char>& input, int64_t idx, int64_t stride)
{
    int64_t spot_on_line = idx % stride;

    auto top_left   = spot_on_line > 0 ? (is_occupied(input, idx - stride - 1) ? 1 : 0) : 0;
    auto top_middle = is_occupied(input, idx - stride) ? 1 : 0;
    auto top_right = spot_on_line < stride - 1 ? (is_occupied(input, idx - stride + 1) ? 1 : 0)
                                               : 0;

    auto left  = spot_on_line > 0 ? (is_occupied(input, idx - 1) ? 1 : 0) : 0;
    auto right = spot_on_line < stride - 1 ? (is_occupied(input, idx + 1) ? 1 : 0) : 0;

    auto bottom_left   = spot_on_line > 0 ? (is_occupied(input, idx + stride - 1) ? 1 : 0) : 0;
    auto bottom_middle = is_occupied(input, idx + stride) ? 1 : 0;
    auto bottom_right  = spot_on_line < stride - 1
                            ? (is_occupied(input, idx + stride + 1) ? 1 : 0)
                            : 0;

    auto counter = top_left + top_middle + top_right + left + right + bottom_left
                   + bottom_middle + bottom_right;

    return counter;
}

void print_map(const std::vector<char>& input, int64_t stride)
{
    for (auto line : input | rv::chunk(stride)) {
        fmt::print("{}\n", line | ranges::to<std::string>);
    }

    fmt::print("\n\n");
}

int64_t part1(std::vector<char> input, int64_t stride)
{
    int64_t last_counter, current_counter = ranges::count(input, '#');

    do {
        last_counter = current_counter;

        print_map(input, stride);

        auto tmp = input;

        input = rv::enumerate(input) | rv::transform([&tmp, stride](auto&& rng) {
                    auto [idx, c] = rng;

                    if (c != '.') {
                        auto occupied_adjacents = count_occupied_adjacent(tmp, idx, stride);

                        if (c == 'L' && occupied_adjacents == 0) {
                            c = '#';
                        }
                        else if (c == '#' && occupied_adjacents >= 4) {
                            c = 'L';
                        }
                    }

                    return c;
                })
                | ranges::to<std::vector>;

        current_counter = ranges::count(input, '#');
    } while (last_counter != current_counter);

    return current_counter;
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 11\n");

    auto [input, stride] = read_input(std::ifstream{"days/day11/input.txt"});

    fmt::print("Part 1 Solution: {}\n", part1(input, stride));
    fmt::print("Part 2 Solution: {}\n", part2());

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

    SECTION("Can solve part 2 example") { REQUIRE(0 == part2()); }
}

#endif
