#include <fmt/format.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <glm/vec3.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <unordered_map>

namespace rs = ranges;
namespace rv = ranges::views;

using grid_t  = std::unordered_map<glm::ivec3, bool>;
using grid2_t = std::unordered_map<glm::ivec4, bool>;

grid_t read_input_grid(std::istream&& input)
{
    glm::ivec3 expanse = {0, 0, 0};

    std::string tmp;
    grid_t      grid;

    while (std::getline(input, tmp)) {
        expanse.x = 0;
        for (auto c : tmp) {
            grid[expanse] = (c == '#');
            ++expanse.x;
        }
        ++expanse.y;
    }

    for (int tmp_z = -1; tmp_z <= 1; ++tmp_z) {
        for (int tmp_y = -1; tmp_y <= expanse.y; ++tmp_y) {
            for (int tmp_x = -1; tmp_x <= expanse.x; ++tmp_x) {
                glm::ivec3 pos = {tmp_x, tmp_y, tmp_z};
                if (!grid.contains(pos)) { grid[pos] = false; }
            }
        }
    }

    return grid;
}

grid2_t read_input_grid2(std::istream&& input)
{
    glm::ivec4 expanse = {0, 0, 0, 0};

    std::string tmp;
    grid2_t     grid;

    while (std::getline(input, tmp)) {
        expanse.x = 0;
        for (auto c : tmp) {
            grid[expanse] = (c == '#');
            ++expanse.x;
        }
        ++expanse.y;
    }

    for (int tmp_w = -1; tmp_w <= 1; ++tmp_w) {
        for (int tmp_z = -1; tmp_z <= 1; ++tmp_z) {
            for (int tmp_y = -1; tmp_y <= expanse.y; ++tmp_y) {
                for (int tmp_x = -1; tmp_x <= expanse.x; ++tmp_x) {
                    glm::ivec4 pos = {tmp_x, tmp_y, tmp_z, tmp_w};
                    if (!grid.contains(pos)) { grid[pos] = false; }
                }
            }
        }
    }

    return grid;
}

int count_active_neighbors(grid_t& grid, glm::ivec3 p)
{
    int active = 0;

    for (int z = p.z - 1; z <= p.z + 1; ++z) {
        for (int y = p.y - 1; y <= p.y + 1; ++y) {
            for (int x = p.x - 1; x <= p.x + 1; ++x) {
                if (p != glm::ivec3{x, y, z}) { active += grid[glm::ivec3{x, y, z}] ? 1 : 0; }
            }
        }
    }

    return active;
}

int count_active_neighbors2(grid2_t& grid, glm::ivec4 p)
{
    int active = 0;

    for (int w = p.w - 1; w <= p.w + 1; ++w) {
        for (int z = p.z - 1; z <= p.z + 1; ++z) {
            for (int y = p.y - 1; y <= p.y + 1; ++y) {
                for (int x = p.x - 1; x <= p.x + 1; ++x) {
                    if (p != glm::ivec4{x, y, z, w}) { active += grid[glm::ivec4{x, y, z, w}] ? 1 : 0; }
                }
            }
        }
    }

    return active;
}

int64_t count_active(const grid_t& g)
{
    return rs::count(g | rv::values, true);
}

int64_t count_active2(const grid2_t& g)
{
    return rs::count(g | rv::values, true);
}

void run_cycle(grid_t& grid)
{
    auto tmp = grid;

    // loop through tmp and check against grid
    for (auto& p : tmp) {
        auto active_neighbors = count_active_neighbors(grid, p.first);

        if (p.second) {
            if (active_neighbors != 2 && active_neighbors != 3) { p.second = false; }
        }
        else {
            if (active_neighbors == 3) { p.second = true; }
        }
    }

    for (auto& p : grid) {
        p.second = tmp[p.first];
    }
}

void run_cycle2(grid2_t& grid)
{
    auto tmp = grid;

    // loop through tmp and check against grid
    for (auto& p : tmp) {
        auto active_neighbors = count_active_neighbors2(grid, p.first);

        if (p.second) {
            if (active_neighbors != 2 && active_neighbors != 3) { p.second = false; }
        }
        else {
            if (active_neighbors == 3) { p.second = true; }
        }
    }

    for (auto& p : grid) {
        p.second = tmp[p.first];
    }
}

int64_t part1(grid_t input)
{
    for (int i : rv::iota(0, 6)) {
        run_cycle(input);
    }

    return count_active(input);
}

int64_t part2(grid2_t input)
{
    for (int i : rv::iota(0, 6)) {
        run_cycle2(input);
    }

    return count_active2(input);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 17\n");

    std::string input_path = "days/day17/puzzle.in";

    fmt::print("Part 1 Solution: {}\n", part1(read_input_grid(std::ifstream{input_path})));
    fmt::print("Part 2 Solution: {}\n", part2(read_input_grid2(std::ifstream{input_path})));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Reading new position creates empty entry")
{
    grid_t grid;

    REQUIRE(0 == grid.size());

    REQUIRE_FALSE(grid[glm::ivec3{0, 0, 0}]);
}

TEST_CASE("Can read puzzle input")
{

    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid(std::move(ss));

    REQUIRE_FALSE(grid[glm::ivec3{0, 0, 0}]);
    REQUIRE(grid[glm::ivec3{2, 2, 0}]);
}

TEST_CASE("Can count active elements")
{

    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid(std::move(ss));

    REQUIRE(5 == count_active(grid));
}

TEST_CASE("Can count active neighbors")
{

    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid(std::move(ss));

    REQUIRE(1 == count_active_neighbors(grid, {0, 0, 0}));
    REQUIRE(1 == count_active_neighbors(grid, {1, 0, 0}));
    REQUIRE(2 == count_active_neighbors(grid, {2, 0, 0}));

    REQUIRE(3 == count_active_neighbors(grid, {0, 1, 0}));
    REQUIRE(5 == count_active_neighbors(grid, {1, 1, 0}));
    REQUIRE(3 == count_active_neighbors(grid, {2, 1, 0}));

    REQUIRE(1 == count_active_neighbors(grid, {0, 2, 0}));
    REQUIRE(3 == count_active_neighbors(grid, {1, 2, 0}));
    REQUIRE(2 == count_active_neighbors(grid, {2, 2, 0}));
}

TEST_CASE("Running cycle increases grid size")
{
    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid(std::move(ss));

    REQUIRE(75 == grid.size());

    run_cycle(grid);

    REQUIRE(245 == grid.size());
}

TEST_CASE("Running cycle increases count")
{
    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid(std::move(ss));

    REQUIRE(5 == count_active(grid));

    run_cycle(grid);

    REQUIRE(11 == count_active(grid));
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    REQUIRE(112 == part1(read_input_grid(std::move(ss))));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    REQUIRE(848 == part2(read_input_grid2(std::move(ss))));
}

#endif
