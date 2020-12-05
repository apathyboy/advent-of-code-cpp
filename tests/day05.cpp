
#include <aoc2020/aoc2020.hpp>

#include <catch2/catch.hpp>
#include <range/v3/all.hpp>

#include <string>
#include <string_view>

using namespace aoc2020;

TEST_CASE("Can calculate row id from boarding pass", "[day05]")
{
    REQUIRE(44 == calculate_row_id("FBFBBFFRLR"));
    REQUIRE(70 == calculate_row_id("BFFFBBFRRR"));
    REQUIRE(14 == calculate_row_id("FFFBBBFRRR"));
    REQUIRE(102 == calculate_row_id("BBFFBBFRLL"));
}

TEST_CASE("Can calculate column from boarding pass", "[day05]")
{
    REQUIRE(5 == calculate_column("FBFBBFFRLR"));
    REQUIRE(7 == calculate_column("BFFFBBFRRR"));
    REQUIRE(7 == calculate_column("FFFBBBFRRR"));
    REQUIRE(4 == calculate_column("BBFFBBFRLL"));
}

TEST_CASE("Can calculate seat id from boarding pass", "[day05]")
{
    REQUIRE(357 == calculate_seat_id("FBFBBFFRLR"));
    REQUIRE(567 == calculate_seat_id("BFFFBBFRRR"));
    REQUIRE(119 == calculate_seat_id("FFFBBBFRRR"));
    REQUIRE(820 == calculate_seat_id("BBFFBBFRLL"));
}
