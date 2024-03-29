
#include <aoc/aoc.hpp>

#include <catch2/catch_test_macros.hpp>

#include <sstream>

TEST_CASE("Can read a newline delimited list of ints from input")
{
    std::stringstream ss;

    ss << R"(1721
979
366
299
675
1456)";

    auto output = aoc::read_element_per_line<int>(std::move(ss));

    REQUIRE(6 == output.size());

    REQUIRE(1721 == output[0]);
    REQUIRE(979 == output[1]);
    REQUIRE(366 == output[2]);
    REQUIRE(299 == output[3]);
    REQUIRE(675 == output[4]);
    REQUIRE(1456 == output[5]);
}

TEST_CASE("Can read a character delimited list of ints from input")
{
    std::stringstream ss;

    ss << R"(1721,979,366,299,675,1456)";

    auto output = aoc::split_line_by<int>(ss, ',');

    REQUIRE(6 == output.size());

    REQUIRE(1721 == output[0]);
    REQUIRE(979 == output[1]);
    REQUIRE(366 == output[2]);
    REQUIRE(299 == output[3]);
    REQUIRE(675 == output[4]);
    REQUIRE(1456 == output[5]);
}
