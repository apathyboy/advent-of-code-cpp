
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>


using namespace ranges;
namespace rv = ranges::views;

const auto search_regex   = std::regex(R"((.*) bags contain (.*))");
const auto contents_regex = std::regex(R"((\d+) (.*?) bag)");

struct contained {
    std::string name;
    int         count;
};

struct node {
    std::string            name;
    std::vector<contained> children;
};

bool can_contain_type(
    const std::vector<node>& nodes,
    const node&              check,
    const std::string&       search_type)
{
    if (find_if(
            check.children,
            [&search_type, &check](const auto& c) { return c.name == search_type; })
        != end(check.children)) {
        return true;
    }

    for (const auto& child : check.children) {
        auto iter = find_if(nodes, [&child](const auto& n) { return n.name == child.name; });

        if (iter != end(nodes)) {
            if (can_contain_type(nodes, *iter, search_type))
                return true;
        }
    }

    return false;
}

int count_children(const std::vector<node>& nodes, const node& check, int times)
{
    int sum = 0;
    for (const auto& child : check.children) {
        sum += (child.count * times);

        auto iter = find_if(nodes, [&child](const auto& n) { return n.name == child.name; });

        if (iter != end(nodes)) {
            sum += count_children(nodes, *iter, times * child.count);
        }
    }

    return sum;
}

std::vector<node> parse_input(std::istream&& is)
{
    return getlines(is) | views::transform([](auto&& s) {
               std::smatch         m, m2;
               std::optional<node> n;

               if (std::regex_match(s, m, search_regex)) {
                   n = node{m.str(1)};

                   for (auto tmp = m.str(2); std::regex_search(tmp, m2, contents_regex);) {
                       n->children.push_back({m2.str(2), std::stoi(m2.str(1))});
                       tmp = m2.suffix();
                   }
               }

               return n.value();
           })
           | to<std::vector>;
}

int part1(const std::vector<node>& input, const std::string& bag_type)
{
    auto nodes = input | views::filter([&input, &bag_type](const auto& n) {
                     return n.name != bag_type && can_contain_type(input, n, bag_type);
                 })
                 | views::transform([](auto&& n) { return n.name; }) | views::unique;
    return static_cast<int>(distance(nodes));
}

int part2(const std::vector<node>& input, const std::string& bag_type)
{
    auto wanted = input
                  | views::filter([&bag_type](const auto& n) { return n.name == bag_type; });

    return count_children(input, front(wanted), 1);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 07\n");

    auto input = parse_input(std::ifstream{"days/day07/puzzle.in"});

    fmt::print("Part 1 Solution: {}\n", part1(input, "shiny gold"));

    fmt::print("Part 2 Solution: {}\n", part2(input, "shiny gold"));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>


TEST_CASE("Can solve day 10 problems")
{
    std::stringstream ss1;

    ss1 << R"(light red bags contain 1 bright white bag, 2 muted yellow bags.
dark orange bags contain 3 bright white bags, 4 muted yellow bags.
bright white bags contain 1 shiny gold bag.
muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.
shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.
dark olive bags contain 3 faded blue bags, 4 dotted black bags.
vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.
faded blue bags contain no other bags.
dotted black bags contain no other bags.)";

    auto input1 = parse_input(std::move(ss1));

    SECTION("Can solve part 1 examples") { REQUIRE(4 == part1(input1, "shiny gold")); }

    SECTION("Can solve part 2 examples")
    {
        std::stringstream ss2;

        ss2 << R"(shiny gold bags contain 2 dark red bags.
dark red bags contain 2 dark orange bags.
dark orange bags contain 2 dark yellow bags.
dark yellow bags contain 2 dark green bags.
dark green bags contain 2 dark blue bags.
dark blue bags contain 2 dark violet bags.
dark violet bags contain no other bags.)";

        auto input2 = parse_input(std::move(ss2));

        REQUIRE(32 == part2(input1, "shiny gold"));
        REQUIRE(126 == part2(input2, "shiny gold"));
    }
}

#endif
