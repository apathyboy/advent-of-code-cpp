
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>


using namespace ranges;
namespace rv = ranges::views;

const auto search_regex   = std::regex(R"((.*) bags contain (.*))");
const auto contents_regex = std::regex(R"((\d+) (.*?) bag)");

struct node_t {
    struct child_t {
        std::string name;
        int         count;
    };

    std::string          name;
    std::vector<child_t> children;
};

int main()
{
    fmt::print("Advent of Code 2020 - Day 07\n");

    std::ifstream ifs{"days/day07/input.txt"};

    const std::string bag_type = "shiny gold";

    auto parts = getlines(ifs) | rv::transform([](auto&& s) {
                     auto matches = s | rv::tokenize(search_regex, {1, 2}) | to<std::vector>();

                     node_t n;
                     n.name   = matches[0].str();
                     auto tmp = matches[1].str();

                     for (auto&& child_data : tmp | rv::tokenize(contents_regex, {1, 2})
                                                  | rv::chunk(2) | to<std::vector>()) {
                         auto t = child_data | to<std::vector>();
                         n.children.push_back({t[1].str(), std::stoi(t[0].str())});
                     }

                     return 0;
                 });

    for (auto part : parts) {
        (part);
        // fmt::print("{} - {}\n", part.str(1), part.str(2));
    }

    return 0;
}
