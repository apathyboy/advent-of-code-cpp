#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

int find_max_y(int y_min)
{
    auto ypos_func = [](int steps, int y_start) { return steps * y_start - steps * (steps - 1) / 2; };

    int vy_max = -y_min - 1;
    int steps  = vy_max + 1;
    int max_y  = ypos_func(steps, vy_max);

    return max_y;
}

int part1(int y_min)
{
    return find_max_y(y_min);
}

int part2(int x_min, int x_max, int y_min, int y_max)
{
    int vy_max = -y_min - 1;

    auto reaches = [x_min, x_max, y_min, y_max](int vx, int vy) {
        int posx = 0, posy = 0;
        while (true) {
            posx += vx;
            posy += vy;
            if (posx >= x_min && posx <= x_max && posy >= y_min && posy <= y_max) return true;
            vy--;
            if (vx > 0) vx--;
            if (posy < y_min) break;
            if (posx > x_max) break;
            if (vx == 0 && posx < x_min) break;
        }
        return false;
    };

    int cnt = 0;
    for (int vy = y_min; vy <= vy_max; vy++) {
        for (int vx = 1; vx <= x_max; vx++) {
            if (reaches(vx, vy)) cnt++;
        }
    }
    return cnt;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 17\n");

    std::ifstream ifs{"puzzle.in"};

    fmt::print("Part 1 Solution: {}\n", part1(-103));
    fmt::print("Part 2 Solution: {}\n", part2(265, 287, -103, -58));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    REQUIRE(45 == part1(-10));
}

TEST_CASE("Can solve part 2 example")
{
    REQUIRE(112 == part2(20, 30, -10, -5));
}

#endif