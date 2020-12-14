#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <bitset>
#include <fstream>
#include <map>
#include <regex>

namespace rs = ranges;
namespace rv = ranges::views;

std::string string_int_to_bits(const std::string& s)
{
    return std::bitset<36>{static_cast<uint64_t>(std::stoll(s))}.to_string();
}

int64_t masked_string_to_num(const std::string& mask, const std::string& value)
{
    // clang-format off
    auto result = rv::zip(value, mask) 
        | rv::transform([](auto&& p) { return (p.second == 'X') ? p.first : p.second; })
        | rs::to<std::string>;
    // clang-format on

    int64_t val = std::stoll(result, nullptr, 2);

    return val;
}

int64_t part1(std::istream&& input)
{
    std::string tmp;
    std::smatch m;

    std::map<int, int64_t> nums;
    std::string            current_mask;

    while (std::getline(input, tmp)) {
        if (tmp.substr(0, 4) == "mask") { current_mask = tmp.substr(7); }
        else {
            if (!std::regex_match(tmp, m, std::regex{R"(mem\[(\d+)\] = (\d+))"})) {
                throw std::runtime_error{"Invalid input received"};
            }

            auto idx = std::stoi(m.str(1));
            auto num = masked_string_to_num(current_mask, string_int_to_bits(m.str(2)));

            nums[idx] = num;
        }
    }

    return rs::accumulate(nums | rv::values, int64_t{0});
}

int part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 14\n");

    fmt::print("Part 1 Solution: {}\n", part1(std::ifstream{"days/day14/puzzle.in"}));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can convert string integer to 32 bit representation")
{
    REQUIRE(string_int_to_bits("11") == "000000000000000000000000000000001011");
    REQUIRE(string_int_to_bits("101") == "000000000000000000000000000001100101");
    REQUIRE(string_int_to_bits("73") == "000000000000000000000000000001001001");
}

TEST_CASE("Can convert masked string to int")
{
    std::string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X";

    REQUIRE(masked_string_to_num(mask, "000000000000000000000000000000001011") == 73);
    REQUIRE(masked_string_to_num(mask, "000000000000000000000000000001100101") == 101);
    REQUIRE(masked_string_to_num(mask, "000000000000000000000000000000000000") == 64);
}

TEST_CASE("Can solve day 14 problems")
{
    std::stringstream ss;

    ss << R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11
mem[7] = 101
mem[8] = 0)";

    SECTION("Can solve part 1 example") { REQUIRE(165 == part1(std::move(ss))); }

    SECTION("Can solve part 2 example") { REQUIRE(0 == part2()); }
}

#endif
