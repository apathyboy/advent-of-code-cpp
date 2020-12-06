
#include <aoc2020/aoc2020.hpp>

#include <catch2/catch.hpp>
#include <range/v3/all.hpp>

#include <string>
#include <string_view>

using namespace aoc2020;

TEST_CASE("Can calculate seat id from boarding pass", "[day05]")
{
    REQUIRE(357 == calculate_seat_id("FBFBBFFRLR"));
    REQUIRE(567 == calculate_seat_id("BFFFBBFRRR"));
    REQUIRE(119 == calculate_seat_id("FFFBBBFRRR"));
    REQUIRE(820 == calculate_seat_id("BBFFBBFRLL"));
}
