#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_map>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;

struct rule {
    enum class TYPE { MATCH, SUBRULE };
    TYPE                          type;
    int                           id;
    std::vector<std::vector<int>> subrules;
    char                          match;
};

auto read_rules(std::istream& input)
{
    std::string                   tmp;
    std::unordered_map<int, rule> rules;

    while (std::getline(input, tmp)) {
        if (tmp == "") break;

        std::smatch m;
        rule        r{rule::TYPE::SUBRULE, 0, {}, ' '};

        if (std::regex_match(tmp, m, std::regex(R"((\d+): \"([a|b])\")"))) {
            r.id    = std::stoi(m.str(1));
            r.type  = rule::TYPE::MATCH;
            r.match = m.str(2)[0];
        }
        else if (std::regex_match(tmp, m, std::regex(R"((\d+):(.*))"))) {
            r.id = std::stoi(m.str(1));
            tmp  = m.str(2);

            r.subrules = tmp | rv::split('|')
                         | rv::transform([](auto&& s) { return s | rs::to<std::string>; })
                         | rv::transform([](auto&& s) {
                               return s | rv::trim([](uint8_t c) { return std::isspace(c); })
                                      | rs::to<std::string>;
                           })
                         | rv::cache1 | rv::transform([](auto&& s) {
                               return s | rv::split(' ') | rv::transform([](auto&& s) {
                                          return std::stoi(s | rs::to<std::string>);
                                      });
                           })
                         | rs::to<std::vector<std::vector<int>>>;
        }

        rules.emplace(std::make_pair(r.id, r));
    }

    return rules;
}

auto read_input(std::istream&& input)
{
    auto rules = read_rules(input);
    return std::make_pair(rules, rs::getlines(input) | rs::to<std::vector<std::string>>);
}

bool match(const std::unordered_map<int, rule>& rules, const rule& r, const std::string& s, int64_t& idx)
{
    if (r.type == rule::TYPE::MATCH) { return s[idx++] == r.match; }

    return rs::any_of(r.subrules, [&rules, &s, &idx, &r](const auto& sr) {
        int64_t tmp          = idx;
        bool    is_recursing = false;
        if (!rs::all_of(sr | rv::enumerate, [&rules, &s, &idx, &r, &is_recursing](const auto& p) {
                if (idx == static_cast<int64_t>(s.length())) return is_recursing;

                auto next_rule = rules.at(p.second);
                is_recursing   = r.id == next_rule.id;
                return match(rules, next_rule, s, idx);
            })) {
            idx = tmp;
            return false;
        }
        return true;
    });
}

bool match(const std::unordered_map<int, rule>& rules, const rule& r, const std::string& s)
{
    int64_t idx = 0;
    return match(rules, r, s, idx) && idx == static_cast<int64_t>(s.size());
}

int64_t part1(const std::unordered_map<int, rule>& rules, const std::vector<std::string>& messages)
{
    return rs::distance(
        messages | rv::filter([&rules](const auto& s) { return match(rules, rules.at(0), s); }));
}

int64_t part2(std::unordered_map<int, rule> rules, const std::vector<std::string>& messages)
{
    rule r8{rule::TYPE::SUBRULE, 8, std::vector{std::vector{42}, std::vector{42, 8}}, ' '};
    rule r11{rule::TYPE::SUBRULE, 11, std::vector{std::vector{42, 31}, std::vector{42, 11, 31}}, ' '};

    rules[8]  = r8;
    rules[11] = r11;

    return rs::distance(
        messages | rv::filter([&rules](const auto& s) { return match(rules, rules.at(0), s); }));
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 19\n");

    std::string input_path = "days/day19/puzzle.in";

    auto [rules, messages] = read_input(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1(rules, messages));
    fmt::print("Part 2 Solution: {}\n", part2(rules, messages));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can match part2 string")
{
    std::stringstream ss;

    ss << R"(42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba)";

    auto [rules, messages] = read_input(std::move(ss));

    rule r8{rule::TYPE::SUBRULE, 8, std::vector{std::vector{42}, std::vector{42, 8}}, ' '};
    rule r11{rule::TYPE::SUBRULE, 11, std::vector{std::vector{42, 31}, std::vector{42, 11, 31}}, ' '};

    rules[8]  = r8;
    rules[11] = r11;

    REQUIRE_FALSE(match(rules, rules[0], messages[11]));
    REQUIRE(match(rules, rules[0], messages[14]));
}

TEST_CASE("Can match string")
{
    std::stringstream ss;

    ss << R"(0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b"

ababbb
bababa
abbbab
aaabbb
aaaabbb)";

    auto [rules, messages] = read_input(std::move(ss));

    REQUIRE(match(rules, rules[0], messages[0]));
    REQUIRE_FALSE(match(rules, rules[0], messages[1]));
    REQUIRE(match(rules, rules[0], messages[2]));
    REQUIRE_FALSE(match(rules, rules[0], messages[3]));
    REQUIRE_FALSE(match(rules, rules[0], messages[4]));
}

TEST_CASE("Can read input")
{
    std::stringstream ss;

    ss << R"(0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b"

ababbb
bababa
abbbab
aaabbb
aaaabbb)";

    auto [rules, messages] = read_input(std::move(ss));

    REQUIRE(6 == rules.size());
    REQUIRE(5 == messages.size());

    REQUIRE(rule::TYPE::SUBRULE == rules[0].type);
    REQUIRE(std::vector{4, 1, 5} == rules[0].subrules[0]);
    REQUIRE(std::vector{2, 3} == rules[1].subrules[0]);
    REQUIRE(std::vector{3, 2} == rules[1].subrules[1]);

    REQUIRE(rule::TYPE::MATCH == rules[4].type);
    REQUIRE('a' == rules[4].match);

    REQUIRE("bababa" == messages[1]);
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(0: 4 1 5
1: 2 3 | 3 2
2: 4 4 | 5 5
3: 4 5 | 5 4
4: "a"
5: "b"

ababbb
bababa
abbbab
aaabbb
aaaabbb)";

    auto [rules, messages] = read_input(std::move(ss));

    REQUIRE(2 == part1(rules, messages));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(42: 9 14 | 10 1
9: 14 27 | 1 26
10: 23 14 | 28 1
1: "a"
11: 42 31
5: 1 14 | 15 1
19: 14 1 | 14 14
12: 24 14 | 19 1
16: 15 1 | 14 14
31: 14 17 | 1 13
6: 14 14 | 1 14
2: 1 24 | 14 4
0: 8 11
13: 14 3 | 1 12
15: 1 | 14
17: 14 2 | 1 7
23: 25 1 | 22 14
28: 16 1
4: 1 1
20: 14 14 | 1 15
3: 5 14 | 16 1
27: 1 6 | 14 18
14: "b"
21: 14 1 | 1 14
25: 1 1 | 1 14
22: 14 14
8: 42
26: 14 22 | 1 20
18: 15 15
7: 14 5 | 1 21
24: 14 1

abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa
bbabbbbaabaabba
babbbbaabbbbbabbbbbbaabaaabaaa
aaabbbbbbaaaabaababaabababbabaaabbababababaaa
bbbbbbbaaaabbbbaaabbabaaa
bbbababbbbaaaaaaaabbababaaababaabab
ababaaaaaabaaab
ababaaaaabbbaba
baabbaaaabbaaaababbaababb
abbbbabbbbaaaababbbbbbaaaababb
aaaaabbaabaaaaababaa
aaaabbaaaabbaaa
aaaabbaabbaaaaaaabbbabbbaaabbaabaaa
babaaabbbaaabaababbaabababaaab
aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba)";

    auto [rules, messages] = read_input(std::move(ss));

    REQUIRE(12 == part2(rules, messages));
}

#endif
