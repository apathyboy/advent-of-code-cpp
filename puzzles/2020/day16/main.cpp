#include <fmt/format.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 28278)
#endif
#include <range/v3/all.hpp>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

#include <fstream>
#include <iostream>
#include <set>

namespace rs = ranges;
namespace rv = ranges::views;

struct rule {
    std::string                        name;
    std::array<std::pair<int, int>, 2> valid_ranges;
};

struct document {
    std::vector<rule>             rules;
    std::vector<int>              ticket;
    std::vector<std::vector<int>> nearby_tickets;
};

std::vector<int> string_to_ints(const std::string& s)
{
    return s | rv::split(',')
           | rv::transform([](auto&& s) { return std::stoi(s | rs::to<std::string>); }) | rs::to_vector;
}

std::vector<rule> read_input_rules(std::istream& input)
{
    std::vector<rule> rules;

    std::smatch m;
    std::string tmp;

    while (std::getline(input, tmp)) {
        if (tmp == "") break;
        if (std::regex_match(tmp, m, std::regex{R"((.*): (\d+)-(\d+) or (\d+)-(\d+))"})) {
            rules.push_back(rule{
                m.str(1),
                {std::pair{std::stoi(m.str(2)), std::stoi(m.str(3))},
                 std::pair{std::stoi(m.str(4)), std::stoi(m.str(5))}}});
        }
    }
    return rules;
}

std::vector<int> read_input_ticket(std::istream& input)
{
    std::string tmp, ticket;
    std::getline(input, tmp);
    std::getline(input, ticket);
    std::getline(input, tmp);
    return string_to_ints(ticket);
}

std::vector<std::vector<int>> read_input_nearby_tickets(std::istream& input)
{
    std::string tmp;
    std::getline(input, tmp);
    return rs::getlines(input) | rv::transform(string_to_ints) | rs::to_vector;
}

document read_input(std::istream&& input)
{
    return {read_input_rules(input), read_input_ticket(input), read_input_nearby_tickets(input)};
}

bool match_rule(const rule& r, int field_value)
{
    return rs::any_of(r.valid_ranges, [field_value](const auto& range) {
        return field_value >= range.first && field_value <= range.second;
    });
}

bool is_valid_field(const std::vector<rule>& rules, int field_value)
{
    return rs::any_of(rules, [field_value](const auto& rule) { return match_rule(rule, field_value); });
}

bool is_valid_ticket(const std::vector<rule>& rules, const std::vector<int>& ticket)
{
    return rs::all_of(ticket, [&rules](int n) { return is_valid_field(rules, n); });
}

auto gather_valid_tickets(const std::vector<rule>& rules, const std::vector<std::vector<int>>& tickets)
{
    return tickets | rv::filter([&rules](const auto& t) { return is_valid_ticket(rules, t); })
           | rs::to_vector;
}

auto transpose_tickets(const std::vector<std::vector<int>>& tickets)
{
    return rv::iota(0, static_cast<int>(tickets[0].size())) | rv::transform([&tickets](int i) {
               return tickets | rv::transform([i](auto&& t) { return t[i]; }) | rs::to_vector;
           })
           | rs::to_vector;
}

int64_t part1(const document& input)
{
    return rs::accumulate(
        input.nearby_tickets | rv::join
            | rv::filter([&input](int i) { return !is_valid_field(input.rules, i); }),
        0);
}

int64_t part2(const document& input, const std::string& search_field)
{
    auto    rules  = input.rules;
    int64_t result = 1;

    auto columns = transpose_tickets(gather_valid_tickets(input.rules, input.nearby_tickets));

    while (rules.size() > 0) {
        for (auto&& [idx, column] : columns | rv::enumerate) {
            auto matching_rules = rules | rv::filter([c = std::ref(column)](const auto& r) {
                                      return rs::all_of(c.get(), [&r](auto n) {
                                          return match_rule(r, n);
                                      });
                                  });

            if (rs::distance(matching_rules) == 1) {
                auto match = rs::front(matching_rules);
                if (match.name.rfind(search_field, 0) == 0) { result *= input.ticket[idx]; }
                rs::erase(
                    rules,
                    rs::remove_if(rules, [&match](const auto& r) { return r.name == match.name; }),
                    rs::end(rules));
            }
        }
    }

    return result;
}


#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 16\n");

    std::string input_path = "days/day16/puzzle.in";

    auto input = read_input(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input, "departure"));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can read field rules from input")
{
    std::stringstream ss;

    ss << R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50
)";

    auto rules = read_input_rules(ss);

    REQUIRE(3 == rules.size());
    REQUIRE("class" == rules[0].name);
    REQUIRE(2 == rules[0].valid_ranges.size());
    REQUIRE(1 == rules[0].valid_ranges[0].first);
    REQUIRE(3 == rules[0].valid_ranges[0].second);
}

TEST_CASE("Can read ticket from input")
{
    std::stringstream ss;

    ss << R"(your ticket:
7,1,14
)";

    auto ticket = read_input_ticket(ss);

    REQUIRE(3 == ticket.size());
    REQUIRE(7 == ticket[0]);
    REQUIRE(1 == ticket[1]);
    REQUIRE(14 == ticket[2]);
}

TEST_CASE("Can read nearby tickets from input")
{
    std::stringstream ss;

    ss << R"(nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)";

    auto nearby_tickets = read_input_nearby_tickets(ss);

    REQUIRE(4 == nearby_tickets.size());
    REQUIRE(3 == nearby_tickets[2].size());
    REQUIRE(55 == nearby_tickets[2][0]);
    REQUIRE(2 == nearby_tickets[2][1]);
    REQUIRE(20 == nearby_tickets[2][2]);
}

TEST_CASE("Can read input")
{
    std::stringstream ss;

    ss << R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)";

    auto document = read_input(std::move(ss));

    REQUIRE(3 == document.rules.size());
    REQUIRE("class" == document.rules[0].name);
    REQUIRE(2 == document.rules[0].valid_ranges.size());
    REQUIRE(1 == document.rules[0].valid_ranges[0].first);
    REQUIRE(3 == document.rules[0].valid_ranges[0].second);

    REQUIRE(3 == document.ticket.size());
    REQUIRE(7 == document.ticket[0]);
    REQUIRE(1 == document.ticket[1]);
    REQUIRE(14 == document.ticket[2]);

    REQUIRE(4 == document.nearby_tickets.size());
    REQUIRE(3 == document.nearby_tickets[2].size());
    REQUIRE(55 == document.nearby_tickets[2][0]);
    REQUIRE(2 == document.nearby_tickets[2][1]);
    REQUIRE(20 == document.nearby_tickets[2][2]);
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(class: 1-3 or 5-7
row: 6-11 or 33-44
seat: 13-40 or 45-50

your ticket:
7,1,14

nearby tickets:
7,3,47
40,4,50
55,2,20
38,6,12)";

    auto input = read_input(std::move(ss));

    REQUIRE(71 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(class: 0-1 or 4-19
row: 0-5 or 8-19
seat: 0-13 or 16-19

your ticket:
11,12,13

nearby tickets:
3,9,18
15,1,5
5,14,9)";

    auto input = read_input(std::move(ss));

    REQUIRE(12 == part2(input, "class"));
}

#endif
