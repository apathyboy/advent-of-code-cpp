#include <fmt/format.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <glm/vec3.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <unordered_map>

namespace rs = ranges;
namespace rv = ranges::views;

template <typename T>
using grid_t = std::unordered_map<T, bool>;

template <typename T>
void pad_grid(grid_t<T>& grid, T max_xy)
{
    for (int tmp_z = -1; tmp_z <= 1; ++tmp_z) {
        for (int tmp_y = -1; tmp_y <= max_xy.y; ++tmp_y) {
            for (int tmp_x = -1; tmp_x <= max_xy.x; ++tmp_x) {
                if constexpr (max_xy.length() == 4) {
                    for (int tmp_w = -1; tmp_w <= 1; ++tmp_w) {
                        glm::ivec4 pos = {tmp_x, tmp_y, tmp_z, tmp_w};
                        if (!grid.contains(pos)) { grid[pos] = false; }
                    }
                }
                else {
                    glm::ivec3 pos = {tmp_x, tmp_y, tmp_z};
                    if (!grid.contains(pos)) { grid[pos] = false; }
                }
            }
        }
    }
}

template <typename T>
grid_t<T> read_input_grid(std::istream&& input)
{
    T max_xy = {};

    std::string tmp;
    grid_t<T>   grid;

    while (std::getline(input, tmp)) {
        max_xy.x = 0;
        for (auto c : tmp) {
            grid[max_xy] = (c == '#');
            ++max_xy.x;
        }
        ++max_xy.y;
    }

    pad_grid(grid, max_xy);

    return grid;
}

template <typename T>
int count_active_neighbors(grid_t<T>& grid, T p)
{
    int active = 0;

    for (int z = p.z - 1; z <= p.z + 1; ++z) {
        for (int y = p.y - 1; y <= p.y + 1; ++y) {
            for (int x = p.x - 1; x <= p.x + 1; ++x) {
                if constexpr (p.length() == 4) {
                    for (int w = p.w - 1; w <= p.w + 1; ++w) {
                        if (p != glm::ivec4{x, y, z, w}) {
                            active += grid[glm::ivec4{x, y, z, w}] ? 1 : 0;
                        }
                    }
                }
                else {
                    if (p != glm::ivec3{x, y, z}) { active += grid[glm::ivec3{x, y, z}] ? 1 : 0; }
                }
            }
        }
    }

    return active;
}

template <typename T>
void run_cycle(T& grid)
{
    auto tmp = grid;

    for (auto& p : grid) {
        auto active = count_active_neighbors(tmp, p.first);
        if (p.second && active != 2 && active != 3) { p.second = false; }
        else if (active == 3) {
            p.second = true;
        }
    }

    grid.merge(tmp);
}

template <typename T>
int64_t solve(grid_t<T> grid)
{
    for (int i = 0; i < 6; ++i) {
        run_cycle(grid);
    }

    return rs::count(grid | rv::values, true);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 17\n");

    std::string input_path = "puzzle.in";

    fmt::print("Part 1 Solution: {}\n", solve(read_input_grid<glm::ivec3>(std::ifstream{input_path})));
    fmt::print("Part 2 Solution: {}\n", solve(read_input_grid<glm::ivec4>(std::ifstream{input_path})));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Reading new position creates empty entry")
{
    grid_t<glm::ivec3> grid;

    REQUIRE(0 == grid.size());

    REQUIRE_FALSE(grid[glm::ivec3{0, 0, 0}]);
}

TEST_CASE("Can read puzzle input")
{

    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid<glm::ivec3>(std::move(ss));

    REQUIRE_FALSE(grid[glm::ivec3{0, 0, 0}]);
    REQUIRE(grid[glm::ivec3{2, 2, 0}]);
}

TEST_CASE("Can count active neighbors")
{

    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    auto grid = read_input_grid<glm::ivec3>(std::move(ss));

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

    auto grid = read_input_grid<glm::ivec3>(std::move(ss));

    REQUIRE(75 == grid.size());

    run_cycle(grid);

    REQUIRE(245 == grid.size());
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    REQUIRE(112 == solve(read_input_grid<glm::ivec3>(std::move(ss))));
}

/* Uncomment when better solution is found
TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(.#.
..#
###)";

    REQUIRE(848 == solve(read_input_grid<glm::ivec4>(std::move(ss))));
}
*/

#endif
