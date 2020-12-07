#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <string>

using namespace ranges;

struct field_t {
    std::string tag;
    std::string data;
};

auto combine_chars        = [](auto const& rc) { return rc | to<std::string>; };
auto to_passport_field    = [](const auto& s) { return field_t{s.substr(0, 3), s.substr(4)}; };
auto is_valid_part1_field = [](const field_t& f) { return f.tag != "cid"; };
auto is_valid_part2_field = [](const field_t& f) {
    if (f.tag == "cid") {
        return false;
    }
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
        if (unit != "in" && unit != "cm")
            return false;

        int h = std::stoi(f.data.substr(0, f.data.length() - 2));
        return (unit == "cm" && h >= 150 && h <= 193) || (unit == "in" && h >= 59 && h <= 76);
    }
    else if (f.tag == "hcl") {
        if (f.data.length() != 7)
            return false;

        if (f.data[0] != '#')
            return false;

        for (auto c : f.data.substr(1)) {
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f'))
                return false;
        }
    }
    else if (f.tag == "ecl") {
        return f.data == "amb" || f.data == "blu" || f.data == "brn" || f.data == "gry"
               || f.data == "grn" || f.data == "hzl" || f.data == "oth";
    }
    else if (f.tag == "pid") {
        if (f.data.length() != 9)
            return false;

        for (auto c : f.data) {
            if (c < '0' || c > '9')
                return false;
        }
    }

    return true;
};

int main()
{
    fmt::print("Advent of Code 2020 - Day 04\n");

    std::ifstream ifs{"days/day04/input.txt"};

    std::string str;

    int part1_passports = 0;
    int part2_passports = 0;

    size_t part1_fields = 0;
    size_t part2_fields = 0;

    while (std::getline(ifs, str)) {
        if (str.length() == 0) {
            if (part1_fields == 7)
                ++part1_passports;

            if (part2_fields == 7)
                ++part2_passports;

            part1_fields = 0;
            part2_fields = 0;
        }
        else {
            // clang-format off
            part1_fields += distance(str 
                | views::split(' ') 
                | views::transform(combine_chars)
                | views::transform(to_passport_field)
                | views::filter(is_valid_part1_field));
                
            part2_fields += distance(str 
                | views::split(' ') 
                | views::transform(combine_chars)
                | views::transform(to_passport_field)
                | views::filter(is_valid_part2_field));
            // clang-format on
        }
    }

    fmt::print("Part 1 Solution: {}\n", part1_passports);
    fmt::print("Part 2 Solution: {}\n", part2_passports);

    return 0;
}
