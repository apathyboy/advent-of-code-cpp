
#include <fmt/core.h>
#include <glm/vec2.hpp>
#include <range/v3/all.hpp>

#include <fstream>
#include <numeric>
#include <string>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

int part1(std::istream&& input)
{
    auto movements = rs::getlines(input) | rv::transform([](auto&& r) {
                         std::string dir = r.substr(0, r.find(' '));
                         int         amt = std::stoi(r.substr(r.find(' ') + 1));

                         glm::ivec2 movement{0, 0};

                         if (dir == "up") { movement += glm::ivec2{0, -amt}; }
                         else if (dir == "down") {
                             movement += glm::ivec2{0, amt};
                         }
                         else if (dir == "forward") {
                             movement += glm::ivec2{amt, 0};
                         }

                         return movement;
                     });

    auto pos = rs::accumulate(movements, glm::ivec2{0, 0});
    return pos.x * pos.y;
}


int part2(std::istream&& input)
{
    int aim = 0;

    auto movements = rs::getlines(input) | rv::transform([&aim](auto&& r) {
                         std::string dir = r.substr(0, r.find(' '));
                         int         amt = std::stoi(r.substr(r.find(' ') + 1));

                         glm::ivec2 movement{0, 0};

                         if (dir == "up") { aim -= amt; }
                         else if (dir == "down") {
                             aim += amt;
                         }
                         else if (dir == "forward") {
                             movement += glm::ivec2{amt, amt * aim};
                         }

                         return movement;
                     });

    auto pos = rs::accumulate(movements, glm::ivec2{0, 0});
    return pos.x * pos.y;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 02\n");

    std::string input_path = "puzzle.in";

    fmt::print("Part 1 Solution: {}\n", part1(std::ifstream{input_path}));
    fmt::print("Part 2 Solution: {}\n", part2(std::ifstream{input_path}));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";

    REQUIRE(150 == part1(std::move(ss)));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(forward 5
down 5
forward 8
up 3
down 8
forward 2)";

    REQUIRE(900 == part2(std::move(ss)));
}

#endif