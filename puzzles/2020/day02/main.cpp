#include <fmt/core.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 28278)
#endif
#include <range/v3/all.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <fstream>
#include <sstream>

namespace rs = ranges;
namespace rv = ranges::views;

struct corporate_policy {
    int  min_count;
    int  max_count;
    char target;
};

using password_vec = std::vector<std::pair<corporate_policy, std::string>>;

std::pair<corporate_policy, std::string> parse_password_rule_string(const std::string& str)
{
    char             junk;
    corporate_policy p;
    std::string      s;

    std::stringstream ss{str};

    ss >> p.min_count >> junk >> p.max_count >> p.target >> junk >> s;

    return std::make_pair(p, s);
}

bool is_valid_day2_part1_pw(const std::pair<corporate_policy, std::string>& pw)
{
    auto c = rs::count(pw.second, pw.first.target);
    return (c >= pw.first.min_count && c <= pw.first.max_count);
}

bool is_valid_day2_part2_pw(const std::pair<corporate_policy, std::string>& pw)
{
    const auto& str    = pw.second;
    int         pos1   = pw.first.min_count - 1;
    int         pos2   = pw.first.max_count - 1;
    char        target = pw.first.target;

    return (str[pos1] == target && str[pos2] != target) || (str[pos1] != target && str[pos2] == target);
}

int64_t part1(const password_vec& input)
{
    return rs::distance(input | rv::filter(is_valid_day2_part1_pw));
}

int64_t part2(const password_vec& input)
{
    return rs::distance(input | rv::filter(is_valid_day2_part2_pw));
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 02\n");

    std::ifstream ifs{"puzzle.in"};

    auto input = rs::getlines(ifs) | rv::transform(parse_password_rule_string) | rs::to<std::vector>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can parse string into rule password pair", "[day02]")
{
    {
        auto [policy, password] = parse_password_rule_string("1-3 a: abcde");

        REQUIRE(password == "abcde");
        REQUIRE(policy.min_count == 1);
        REQUIRE(policy.max_count == 3);
        REQUIRE(policy.target == 'a');
    }

    {
        auto [policy, password] = parse_password_rule_string("1-3 b: cdefg");

        REQUIRE(password == "cdefg");
        REQUIRE(policy.min_count == 1);
        REQUIRE(policy.max_count == 3);
        REQUIRE(policy.target == 'b');
    }

    {
        auto [policy, password] = parse_password_rule_string("2-9 c: ccccccccc");

        REQUIRE(password == "ccccccccc");
        REQUIRE(policy.min_count == 2);
        REQUIRE(policy.max_count == 9);
        REQUIRE(policy.target == 'c');
    }
}
TEST_CASE("Can validate part 1 passwords", "[day02]")
{
    REQUIRE(is_valid_day2_part1_pw(parse_password_rule_string("1-3 a: abcde")));
    REQUIRE_FALSE(is_valid_day2_part1_pw(parse_password_rule_string("1-3 b: cdefg")));
    REQUIRE(is_valid_day2_part1_pw(parse_password_rule_string("2-9 c: ccccccccc")));
}

TEST_CASE("Can validate part 2 passwords", "[day02]")
{
    REQUIRE(is_valid_day2_part2_pw(parse_password_rule_string("1-3 a: abcde")));
    REQUIRE_FALSE(is_valid_day2_part1_pw(parse_password_rule_string("1-3 b: cdefg")));
    REQUIRE_FALSE(is_valid_day2_part2_pw(parse_password_rule_string("2-9 c: ccccccccc")));
}

TEST_CASE("Can solve day 2 problems")
{
    std::stringstream ss;

    ss << R"(1-3 a: abcde
1-3 b: cdefg
2-9 c: ccccccccc)";

    auto input = rs::getlines(ss) | rv::transform(parse_password_rule_string) | rs::to<std::vector>;

    SECTION("Can solve part 1 example") { REQUIRE(2 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(1 == part2(input)); }
}

#endif
