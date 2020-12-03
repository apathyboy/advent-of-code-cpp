
#include <aoc2020/aoc2020.hpp>

#include <catch2/catch.hpp>

#include <ranges>
#include <sstream>
#include <string_view>
#include <utility>

TEST_CASE("Can parse string into rule password pair", "[day02]")
{
    {
        auto [policy, password] = aoc2020::parse_password_rule_string("1-3 a: abcde");

        REQUIRE(password == "abcde");
        REQUIRE(policy.min_count == 1);
        REQUIRE(policy.max_count == 3);
        REQUIRE(policy.target == 'a');
    }

    {
        auto [policy, password] = aoc2020::parse_password_rule_string("1-3 b: cdefg");

        REQUIRE(password == "cdefg");
        REQUIRE(policy.min_count == 1);
        REQUIRE(policy.max_count == 3);
        REQUIRE(policy.target == 'b');
    }

    {
        auto [policy, password] = aoc2020::parse_password_rule_string("2-9 c: ccccccccc");

        REQUIRE(password == "ccccccccc");
        REQUIRE(policy.min_count == 2);
        REQUIRE(policy.max_count == 9);
        REQUIRE(policy.target == 'c');
    }
}

TEST_CASE("Can validate part 1 passwords", "[day02]")
{
    {
        auto input = aoc2020::parse_password_rule_string("1-3 a: abcde");
        REQUIRE(aoc2020::is_valid_day2_part1_pw(input));
    }

    {
        auto input = aoc2020::parse_password_rule_string("1-3 b: cdefg");
        REQUIRE_FALSE(aoc2020::is_valid_day2_part1_pw(input));
    }

    {
        auto input = aoc2020::parse_password_rule_string("2-9 c: ccccccccc");
        REQUIRE(aoc2020::is_valid_day2_part1_pw(input));
    }
}

TEST_CASE("Can validate part 2 passwords", "[day02]")
{
    {
        auto input = aoc2020::parse_password_rule_string("1-3 a: abcde");
        REQUIRE(aoc2020::is_valid_day2_part2_pw(input));
    }

    {
        auto input = aoc2020::parse_password_rule_string("1-3 b: cdefg");
        REQUIRE_FALSE(aoc2020::is_valid_day2_part1_pw(input));
    }

    {
        auto input = aoc2020::parse_password_rule_string("2-9 c: ccccccccc");
        REQUIRE_FALSE(aoc2020::is_valid_day2_part2_pw(input));
    }
}
