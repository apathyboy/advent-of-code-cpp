
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>


using namespace ranges;

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

int main()
{
    fmt::print("Advent of Code 2020 - Day 07\n");

    std::ifstream ifs{"days/day07/input.txt"};

    const std::string bag_type = "shiny gold";

    auto input = getlines(ifs) | views::transform([](auto&& s) {
                     std::smatch m;
                     std::regex_match(s, m, search_regex);

                     node n;
                     n.name = m.str(1);

                     auto tmp = m.str(2);

                     std::smatch m2;
                     while (std::regex_search(tmp, m2, contents_regex)) {
                         n.children.push_back({m2.str(2), std::stoi(m2.str(1))});
                         tmp = m2.suffix();
                     }

                     return n;
                 })
                 | to<std::vector<node>>;

    fmt::print("Part 1 Solution: {}\n", part1(input, bag_type));

    fmt::print("Part 2 Solution: {}\n", part2(input, bag_type));

    return 0;
}
