#include <fmt/format.h>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"
#include <glm/vec3.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <queue>
#include <unordered_map>

namespace rs = ranges;
namespace rv = ranges::views;

void pad_grid(std::unordered_map<glm::ivec3, int>& grid)
{
    for (auto [pos, val] : grid) {
        grid[pos + glm::ivec3{1, 0, -1}] = (grid[pos + glm::ivec3{1, 0, -1}] + 2) % 2;
        grid[pos + glm::ivec3{0, 1, -1}] = (grid[pos + glm::ivec3{0, 1, -1}] + 2) % 2;
        grid[pos + glm::ivec3{0, -1, 1}] = (grid[pos + glm::ivec3{0, -1, 1}] + 2) % 2;
        grid[pos + glm::ivec3{-1, 0, 1}] = (grid[pos + glm::ivec3{-1, 0, 1}] + 2) % 2;
        grid[pos + glm::ivec3{1, -1, 0}] = (grid[pos + glm::ivec3{1, -1, 0}] + 2) % 2;
        grid[pos + glm::ivec3{-1, 1, 0}] = (grid[pos + glm::ivec3{-1, 1, 0}] + 2) % 2;
    }
}

std::unordered_map<glm::ivec3, int> read_input(std::istream& input)
{
    std::unordered_map<glm::ivec3, int> grid;

    auto tile_positions = rs::getlines(input)
                          | rv::transform([](auto&& s) { return s | rs::to<std::string>; })
                          | rv::transform([](auto&& s) {
                                glm::ivec3 pos = {0, 0, 0};

                                int read_idx = 0;

                                while (read_idx < s.length()) {
                                    auto c = s[read_idx++];

                                    if (c == 'n' || c == 's') {
                                        char c2 = s[read_idx++];

                                        if (c == 'n' && c2 == 'e') { pos += glm::ivec3{1, 0, -1}; }
                                        else if (c == 'n' && c2 == 'w') {
                                            pos += glm::ivec3{0, 1, -1};
                                        }
                                        else if (c == 's' && c2 == 'e') {
                                            pos += glm::ivec3{0, -1, 1};
                                        }
                                        else if (c == 's' && c2 == 'w') {
                                            pos += glm::ivec3{-1, 0, 1};
                                        }
                                    }
                                    else if (c == 'e') {
                                        pos += glm::ivec3{1, -1, 0};
                                    }
                                    else {
                                        pos += glm::ivec3{-1, 1, 0};
                                    }
                                }

                                return pos;
                            });

    for (auto pos : tile_positions) {
        grid[pos] = (grid[pos] + 1) % 2;
    }

    pad_grid(grid);

    return grid;
}

std::pair<int, int> count_neighbors(std::unordered_map<glm::ivec3, int>& grid, const glm::ivec3& pos)
{

    int black_neighbors = grid[pos + glm::ivec3{1, 0, -1}] + grid[pos + glm::ivec3{0, 1, -1}]
                          + grid[pos + glm::ivec3{0, -1, 1}] + grid[pos + glm::ivec3{-1, 0, 1}]
                          + grid[pos + glm::ivec3{1, -1, 0}] + grid[pos + glm::ivec3{-1, 1, 0}];

    return {6 - black_neighbors, black_neighbors};
}

int64_t part1(std::unordered_map<glm::ivec3, int> grid)
{
    return rs::count(grid | rv::values, 1);
}

int64_t part2(std::unordered_map<glm::ivec3, int> grid)
{
    int64_t black_tiles = 0;
    for (int i = 0; i < 100; ++i) {
        auto tmp = grid;

        for (auto [pos, tile] : grid) {
            auto [white_neighbors, black_neighbors] = count_neighbors(tmp, pos);

            if (tile == 0) {
                if (black_neighbors == 2) { grid[pos] = 1; }
            }
            else {
                if (black_neighbors == 0 || black_neighbors > 2) { grid[pos] = 0; }
            }
        }

        grid.merge(tmp);

        black_tiles = rs::count(grid | rv::values, 1);
    }

    return black_tiles;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 24\n");

    std::string input_path = "days/day24/puzzle.in";

    std::ifstream ifs{input_path};

    auto grid = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(grid));
    fmt::print("Part 2 Solution: {}\n", part2(grid));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(sesenwnenenewseeswwswswwnenewsewsw
neeenesenwnwwswnenewnwwsewnenwseswesw
seswneswswsenwwnwse
nwnwneseeswswnenewneswwnewseswneseene
swweswneswnenwsewnwneneseenw
eesenwseswswnenwswnwnwsewwnwsene
sewnenenenesenwsewnenwwwse
wenwwweseeeweswwwnwwe
wsweesenenewnwwnwsenewsenwwsesesenwne
neeswseenwwswnwswswnw
nenwswwsewswnenenewsenwsenwnesesenew
enewnwewneswsewnwswenweswnenwsenwsw
sweneswneswneneenwnewenewwneswswnese
swwesenesewenwneswnwwneseswwne
enesenwswwswneneswsenwnewswseenwsese
wnwnesenesenenwwnenwsewesewsesesew
nenewswnwewswnenesenwnesewesw
eneswnwswnwsenenwnwnwwseeswneewsenese
neswnwewnwnwseenwseesewsenwsweewe
wseweeenwnesenwwwswnew)";

    REQUIRE(10 == part1(read_input(ss)));
}

/* Uncomment when better solution is found
TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(sesenwnenenewseeswwswswwnenewsewsw
neeenesenwnwwswnenewnwwsewnenwseswesw
seswneswswsenwwnwse
nwnwneseeswswnenewneswwnewseswneseene
swweswneswnenwsewnwneneseenw
eesenwseswswnenwswnwnwsewwnwsene
sewnenenenesenwsewnenwwwse
wenwwweseeeweswwwnwwe
wsweesenenewnwwnwsenewsenwwsesesenwne
neeswseenwwswnwswswnw
nenwswwsewswnenenewsenwsenwnesesenew
enewnwewneswsewnwswenweswnenwsenwsw
sweneswneswneneenwnewenewwneswswnese
swwesenesewenwneswnwwneseswwne
enesenwswwswneneswsenwnewswseenwsese
wnwnesenesenenwwnenwsewesewsesesew
nenewswnwewswnenesenwnesewesw
eneswnwswnwsenenwnwnwwseeswneewsenese
neswnwewnwnwseenwseesewsenwsweewe
wseweeenwnesenwwwswnew)";

    REQUIRE(2208 == part2(read_input(ss)));
}
*/

#endif
