
#include <aoc2020/aoc2020.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <catch2/catch.hpp>

#include <sstream>
#include <string>

TEST_CASE("Can parse string input of 1 int per line", "[main]")
{
    std::stringstream stream;
    stream << "12345\n"
           << "-2345\n"
           << "147\n";

    auto ints = aoc2020::read_int_per_line(stream);

    REQUIRE(ints.size() == 3);
    REQUIRE(ints[0] == 12345);
    REQUIRE(ints[1] == -2345);
    REQUIRE(ints[2] == 147);
}
