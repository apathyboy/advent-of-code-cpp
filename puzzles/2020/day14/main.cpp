#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <bitset>
#include <fstream>
#include <map>
#include <regex>

namespace rs = ranges;
namespace rv = ranges::views;

std::string parse_to_bits(const std::string& s)
{
    return std::bitset<36>{static_cast<uint64_t>(std::stoll(s))}.to_string();
}

int64_t masked_to_int(const std::string& mask, const std::string& value)
{
    // clang-format off
    auto result = rv::zip(value, mask) 
        | rv::transform([](auto&& p) { return (p.second == 'X') ? p.first : p.second; })
        | rs::to<std::string>;
    // clang-format on

    return std::stoll(result, nullptr, 2);
}

std::vector<int64_t> explode_addresses(const std::string& mask, const std::string& value)
{
    auto masked = rv::zip(value, mask)
                  | rv::transform([](auto&& p) { return (p.second == '0') ? p.first : p.second; })
                  | rs::to<std::string>;

    std::vector<std::string> floating{masked};

    while (rs::find_if(floating, [](const auto& s) { return rs::contains(s, 'X'); })
           != rs::end(floating)) {
        std::vector<std::string> tmp;

        for (auto s : floating) {
            auto pos = s.find_first_of('X');
            if (pos != std::string::npos) {
                s[pos] = '0';
                tmp.push_back(s);
                s[pos] = '1';
                tmp.push_back(s);
            }
            else {
                tmp.push_back(s);
            }
        }

        std::swap(floating, tmp);
    }

    return floating | rv::transform([](auto&& s) { return std::stoll(s, nullptr, 2); })
           | rs::to<std::vector<int64_t>>;
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
            auto num = masked_to_int(current_mask, parse_to_bits(m.str(2)));

            nums[idx] = num;
        }
    }

    return rs::accumulate(nums | rv::values, int64_t{0});
}

int64_t part2(std::istream&& input)
{
    std::string tmp;
    std::smatch m;

    std::map<int64_t, int64_t> nums;
    std::string                current_mask;

    while (std::getline(input, tmp)) {
        if (tmp.substr(0, 4) == "mask") { current_mask = tmp.substr(7); }
        else {
            if (!std::regex_match(tmp, m, std::regex{R"(mem\[(\d+)\] = (\d+))"})) {
                throw std::runtime_error{"Invalid input received"};
            }

            auto addresses = explode_addresses(current_mask, parse_to_bits(m.str(1)));

            for (auto address : addresses) {
                nums[address] = std::stoll(m.str(2));
            }
        }
    }

    return rs::accumulate(nums | rv::values, int64_t{0});
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 14\n");

    std::string input_path = "puzzle.in";

    fmt::print("Part 1 Solution: {}\n", part1(std::ifstream{input_path}));
    fmt::print("Part 2 Solution: {}\n", part2(std::ifstream{input_path}));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can convert string integer to 32 bit representation")
{
    REQUIRE(parse_to_bits("11") == "000000000000000000000000000000001011");
    REQUIRE(parse_to_bits("101") == "000000000000000000000000000001100101");
    REQUIRE(parse_to_bits("73") == "000000000000000000000000000001001001");
}

TEST_CASE("Can convert masked string to int")
{
    std::string mask = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X";

    REQUIRE(masked_to_int(mask, "000000000000000000000000000000001011") == 73);
    REQUIRE(masked_to_int(mask, "000000000000000000000000000001100101") == 101);
    REQUIRE(masked_to_int(mask, "000000000000000000000000000000000000") == 64);
}

TEST_CASE("Can explode address to all its floating addresses")
{
    auto addresses = explode_addresses(
        "000000000000000000000000000000X1001X",
        "000000000000000000000000000000101010");

    REQUIRE(4 == addresses.size());
    REQUIRE(26 == addresses[0]);
    REQUIRE(27 == addresses[1]);
    REQUIRE(58 == addresses[2]);
    REQUIRE(59 == addresses[3]);
}

TEST_CASE("Can solve day 14 problems")
{
    std::stringstream ss;

    SECTION("Can solve part 1 example")
    {
        ss << R"(mask = XXXXXXXXXXXXXXXXXXXXXXXXXXXXX1XXXX0X
mem[8] = 11
mem[7] = 101
mem[8] = 0)";

        REQUIRE(165 == part1(std::move(ss)));
    }

    SECTION("Can solve part 2 example")
    {
        ss << R"(mask = 000000000000000000000000000000X1001X
mem[42] = 100
mask = 00000000000000000000000000000000X0XX
mem[26] = 1)";

        REQUIRE(208 == part2(std::move(ss)));
    }
}

#endif
