
#include <aoc/intcode.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Can run simple intcode program")
{
    std::vector<int> program{1, 9, 10, 3, 2, 3, 11, 0, 99, 30, 40, 50};

    program = aoc::compute(program);

    REQUIRE(3500 == program[0]);
}
