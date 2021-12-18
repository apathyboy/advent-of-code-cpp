#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <unordered_map>
#include <vector>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

template <>
struct std::hash<std::pair<char, char>> {
    std::size_t operator()(std::pair<char, char> const& s) const noexcept
    {
        std::size_t h1 = std::hash<char>{}(s.first);
        std::size_t h2 = std::hash<char>{}(s.second);
        return h1 ^ (h2 << 1); // or use boost::hash_combine
    }
};

using insertion_rules_t = std::unordered_map<std::pair<char, char>, char>;

struct input_t {
    std::string       polymer_template;
    insertion_rules_t insertion_rules;
};

struct frequency_counter {
    frequency_counter(const std::string& input)
        : char_freq(26, 0)
        , pair_freq{}
    {
        std::pair pair{'\0', '\0'};
        for (char c : input) {
            pair.first = std::exchange(pair.second, c);
            if (pair.first != '\0') pair_freq[pair]++;
            char_freq[c - 'A']++;
        }
    }

    void tick(const insertion_rules_t& rules)
    {
        auto new_pair_freq = pair_freq;
        auto new_char_freq = char_freq;

        for (auto& pair : pair_freq) {
            if (!rules.contains(pair.first)) continue;
            char c = rules.at(pair.first);
            new_pair_freq[pair.first] -= pair.second;
            new_char_freq[c - 'A'] += pair.second;

            std::pair left{pair.first.first, c};
            std::pair right{c, pair.first.second};
            new_pair_freq[left] += pair.second;
            new_pair_freq[right] += pair.second;
        }

        pair_freq = std::move(new_pair_freq);
        char_freq = std::move(new_char_freq);
    }

    std::vector<uint64_t>                               char_freq;
    std::unordered_map<std::pair<char, char>, uint64_t> pair_freq;
};

input_t read_input(std::istream& input)
{
    std::string tmp;
    std::getline(input, tmp);

    input_t i{};

    i.polymer_template = tmp;

    std::getline(input, tmp);

    // clang-format off
    i.insertion_rules = rs::getlines(input) 
        | rv::transform([](const auto& s) {
                            return std::pair{std::pair{s[0], s[1]}, s[6]};
                        })
        | rs::to<std::unordered_map>;
    // clang-format on

    return i;
}

int64_t part1(input_t input)
{
    std::string polymer = input.polymer_template;

    for (int i : rv::iota(0, 10)) {
        (i);

        std::string next_polymer{};

        for (auto&& window : polymer | rv::sliding(2)) {
            std::pair tmp{window[0], window[1]};

            auto c = input.insertion_rules[tmp];

            next_polymer.push_back(window[0]);
            next_polymer.push_back(c);
        }

        next_polymer.push_back(polymer.back());

        polymer = next_polymer;
    }

    rs::sort(polymer);

    auto counts = polymer | rv::chunk_by([](char x, char y) { return x == y; })
                  | rv::transform([](const auto& r) { return rs::distance(r); }) | rs::to<std::vector>;

    return rs::max(counts) - rs::min(counts);
}

int64_t part2(input_t input)
{
    frequency_counter counter(input.polymer_template);
    for (int i = 0; i < 40; i++) {
        counter.tick(input.insertion_rules);
    }

    auto non_zero   = counter.char_freq | rv::filter([](auto v) { return v != 0; });
    auto [min, max] = rs::minmax_element(non_zero);

    return *max - *min;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 14\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)";

    auto input = read_input(ss);

    REQUIRE(1588 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(NNCB

CH -> B
HH -> N
CB -> H
NH -> C
HB -> C
HC -> B
HN -> C
NN -> C
BH -> H
NC -> B
NB -> B
BN -> B
BB -> N
BC -> B
CC -> N
CN -> C)";

    auto input = read_input(ss);

    REQUIRE(2188189693529 == part2(input));
}

#endif