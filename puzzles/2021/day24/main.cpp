#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

int64_t search(
    std::vector<int32_t>& divisors,
    std::vector<int32_t>& offsets,
    std::vector<int32_t>& modifiers,
    auto                  op)
{
    std::unordered_map<int64_t, int64_t> z_values;
    std::unordered_map<int64_t, int64_t> z_values_new;
    z_values.insert(std::make_pair(0, 0));

    for (int64_t index = 0; index < 14; index++) {
        for (auto z0 : z_values) {
            for (int64_t digit = 9; digit > 0; digit--) {
                int64_t candidate = z0.second * 10 + digit;
                int64_t z         = z0.first / divisors[index];
                if (z0.first % 26 + offsets[index] != digit) z = z * 26 + digit + modifiers[index];
                auto [it, flag] = z_values_new.emplace(z, candidate);
                if (!flag) it->second = op(it->second, candidate);
            }
        }
        z_values = std::move(z_values_new);
    }
    return z_values[0];
}

int64_t
part1(std::vector<int32_t> divisors, std::vector<int32_t> offsets, std::vector<int32_t> modifiers)
{
    return search(divisors, offsets, modifiers, [](uint64_t a, uint64_t b) { return std::max(a, b); });
}

int64_t
part2(std::vector<int32_t> divisors, std::vector<int32_t> offsets, std::vector<int32_t> modifiers)
{
    return search(divisors, offsets, modifiers, [](uint64_t a, uint64_t b) { return std::min(a, b); });
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 24\n");

    std::ifstream ifs{"puzzle.in"};

    std::vector<int32_t> divisors;
    std::vector<int32_t> offsets;
    std::vector<int32_t> modifiers;

    std::string line;
    size_t      block_id = 0;
    size_t      line_id  = 0;
    while (std::getline(ifs, line)) {
        if (line_id == 4) {
            int32_t val = std::stol(line.substr(6));
            divisors.push_back(val);
        }
        if (line_id == 5) {
            int32_t val = std::stol(line.substr(6));
            offsets.push_back(val);
        }
        if (line_id == 15) {
            int32_t val = std::stol(line.substr(6));
            modifiers.push_back(val);
        }

        line_id++;
        block_id += line_id / 18;
        line_id %= 18;
    }

    fmt::print("Part 1 Solution: {}\n", part1(divisors, offsets, modifiers));
    fmt::print("Part 2 Solution: {}\n", part2(divisors, offsets, modifiers));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"()";

    std::vector<int32_t> divisors;
    std::vector<int32_t> offsets;
    std::vector<int32_t> modifiers;

    REQUIRE(0 == part1(divisors, offsets, modifiers));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"()";

    std::vector<int32_t> divisors;
    std::vector<int32_t> offsets;
    std::vector<int32_t> modifiers;

    REQUIRE(0 == part2(divisors, offsets, modifiers));
}

#endif