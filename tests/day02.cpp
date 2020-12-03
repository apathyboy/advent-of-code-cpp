
#include <aoc2020/aoc2020.hpp>

#include <catch2/catch.hpp>

#include <sstream>
#include <string_view>
#include <utility>

TEST_CASE("Can parse string into rule password pair", "[day02]")
{
    {
        std::string input = "1-3 a: abcde";

        auto [policy, password] = aoc2020::parse_password_rule_string(input);

        REQUIRE(password == "abcde");
        REQUIRE(policy.min_count == 1);
        REQUIRE(policy.max_count == 3);
        REQUIRE(policy.target == 'a');
    }
    {
        std::string input = "1-3 b: cdefg";

        auto [policy, password] = aoc2020::parse_password_rule_string(input);

        REQUIRE(password == "cdefg");
        REQUIRE(policy.min_count == 1);
        REQUIRE(policy.max_count == 3);
        REQUIRE(policy.target == 'b');
    }
    {
        std::string input = "2-9 c: ccccccccc";

        auto [policy, password] = aoc2020::parse_password_rule_string(input);

        REQUIRE(password == "ccccccccc");
        REQUIRE(policy.min_count == 2);
        REQUIRE(policy.max_count == 9);
        REQUIRE(policy.target == 'c');
    }
}
