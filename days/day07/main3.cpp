
#include <boost/algorithm/string/trim.hpp>
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

using namespace ranges;

const auto search_regex   = std::regex(R"((.*) bags contain (.*))");
const auto contents_regex = std::regex(R"((\d+) (.*?) bag)");

struct node {
    std::string                        name;
    std::vector<std::pair<node*, int>> children;
};

int main()
{
    fmt::print("Advent of Code 2020 - Day 07\n");

    std::ifstream ifs{"days/day07/input.txt"};

    std::unordered_map<std::string, std::unique_ptr<node>> cache;

    const std::string bag_type = "shiny gold";
    /*
        std::string tmp;
        while (std::getline(ifs, tmp)) {
            std::smatch m;
            std::regex_match(tmp, m, search_regex);

            node* n;

            if (cache.contains(m.str(1))) {
                n = cache[m.str(1)].get();
            }
            else {
                auto [iter, res] = cache.emplace(m.str(1), std::make_unique<node>(m.str(1)));
                n                = iter->second.get();
            }
        }
        */

    auto input = getlines(ifs) | views::transform([&cache](auto&& s) {
                     std::smatch m;
                     std::regex_match(s, m, search_regex);

                     auto [iter, res] = cache.emplace(
                         m.str(1),
                         std::make_unique<node>(m.str(1)));
                     node* n = iter->second.get();

                     (n);

                     auto children = m.str(2);

                     auto rng = children | views::tokenize(contents_regex, {1, 2})
                                | views::transform([](auto&& m) { return m.str(); })
                                | views::chunk(2) | views::transform([](auto&& rng) {
                                      auto i   = rng.begin();
                                      auto str = *i;
                                      advance(i, 1);
                                      auto str2 = *i;

                                      fmt::print("{} ({})\n", str2, str);

                                      return std::make_pair(std::stoi(str), str2);
                                  });

                     for (auto i : rng) {
                         (i);
                     }

                     // std::cout << rng << std::endl;

                     return false;
                 });

    for (auto i : input) {
        (i);
    }

    size_t bags_containing_target = distance(input);

    fmt::print("Part 1 Solution: {}\n", bags_containing_target);

    return 0;
}
