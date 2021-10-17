#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <string>

namespace rs = ranges;
namespace rv = ranges::views;

struct field_t {
    std::string tag;
    std::string data;
};

auto combine_chars        = [](auto const& rc) { return rc | rs::to<std::string>; };
auto to_passport_field    = [](const auto& s) { return field_t{s.substr(0, 3), s.substr(4)}; };
auto is_valid_part1_field = [](const field_t& f) { return f.tag != "cid"; };
auto is_valid_part2_field = [](const field_t& f) {
    if (f.tag == "cid") { return false; }
    else if (f.tag == "byr") {
        int d = std::stoi(f.data);
        return d >= 1920 && d <= 2002;
    }
    else if (f.tag == "iyr") {
        int d = std::stoi(f.data);
        return d >= 2010 && d <= 2020;
    }
    else if (f.tag == "eyr") {
        int d = std::stoi(f.data);
        return d >= 2020 && d <= 2030;
    }
    else if (f.tag == "hgt") {

        std::string unit = f.data.substr(f.data.length() - 2);
        if (unit != "in" && unit != "cm") return false;

        int h = std::stoi(f.data.substr(0, f.data.length() - 2));
        return (unit == "cm" && h >= 150 && h <= 193) || (unit == "in" && h >= 59 && h <= 76);
    }
    else if (f.tag == "hcl") {
        if (f.data.length() != 7) return false;

        if (f.data[0] != '#') return false;

        for (auto c : f.data.substr(1)) {
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f')) return false;
        }
    }
    else if (f.tag == "ecl") {
        return f.data == "amb" || f.data == "blu" || f.data == "brn" || f.data == "gry"
               || f.data == "grn" || f.data == "hzl" || f.data == "oth";
    }
    else if (f.tag == "pid") {
        if (f.data.length() != 9) return false;

        for (auto c : f.data) {
            if (c < '0' || c > '9') return false;
        }
    }

    return true;
};

int part1(const std::vector<std::string>& lines)
{
    int passports = 0;

    size_t fields = 0;

    for (const auto& str : lines) {
        if (str.length() == 0) {
            if (fields == 7) ++passports;

            fields = 0;
        }
        else {
            // clang-format off
            fields += rs::distance(str 
                | rv::split(' ') 
                | rv::transform(combine_chars)
                | rv::transform(to_passport_field)
                | rv::filter(is_valid_part1_field));
            // clang-format on
        }
    }

    return passports;
}

int part2(const std::vector<std::string>& lines)
{
    int    passports = 0;
    size_t fields    = 0;

    for (const auto& str : lines) {
        if (str.length() == 0) {
            if (fields == 7) ++passports;

            fields = 0;
        }
        else {
            // clang-format off
            fields += rs::distance(str 
                | rv::split(' ') 
                | rv::transform(combine_chars)
                | rv::transform(to_passport_field)
                | rv::filter(is_valid_part2_field));
            // clang-format on
        }
    }

    return passports;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 04\n");

    std::ifstream ifs{"days/day04/puzzle.in"};

    auto input = ranges::getlines(ifs) | ranges::to<std::vector>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve day 4 problems")
{
    std::stringstream ss;

    ss << R"(ecl:gry pid:860033327 eyr:2020 hcl:#fffffd
byr:1937 iyr:2017 cid:147 hgt:183cm

iyr:2013 ecl:amb cid:350 eyr:2023 pid:028048884
hcl:#cfa07d byr:1929

hcl:#ae17e1 iyr:2013
eyr:2024
ecl:brn pid:760753108 byr:1931
hgt:179cm

hcl:#cfa07d eyr:2025 pid:166559648
iyr:2011 ecl:brn hgt:59in)";

    auto input = ranges::getlines(ss) | ranges::to<std::vector>;

    SECTION("Can solve part 1 example") { REQUIRE(2 == part1(input)); }

    SECTION("Can solve part 2 example") { REQUIRE(2 == part2(input)); }
}

#endif
