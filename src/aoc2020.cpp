
#include "aoc2020.hpp"

#include <range/v3/all.hpp>

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <ranges>
#include <sstream>

using namespace ranges;

namespace aoc2020 {

std::vector<int> read_int_per_line(std::istream& input)
{
    std::vector<int> result;
    int              tmp;

    while (input >> tmp) {
        result.push_back(tmp);
    }

    return result;
}

std::vector<std::pair<int, int>> find_sum_to_pairs(const std::vector<int>& vals, int target)
{
    std::vector<std::pair<int, int>> results;

    for (auto n : vals) {
        for (auto m : vals) {
            if (n == m)
                continue;

            if (n + m == target) {
                results.push_back(std::make_pair(n, m));
            }
        }
    }

    return results;
}

std::vector<std::tuple<int, int, int>>
find_sum_to_triplets(const std::vector<int>& vals, int target)
{
    std::vector<std::tuple<int, int, int>> results;

    for (auto n : vals) {
        for (auto m : vals) {
            if (n == m)
                continue;

            for (auto l : vals) {
                if (n == l || m == l)
                    continue;

                if (n + m + l == target) {
                    results.push_back(std::tie(n, m, l));
                }
            }
        }
    }

    return results;
}

std::pair<corporate_policy, std::string> parse_password_rule_string(const std::string& str)
{
    char             junk;
    corporate_policy p;
    std::string      s;

    std::stringstream ss{str};

    ss >> p.min_count >> junk >> p.max_count >> p.target >> junk >> s;

    return std::make_pair(p, s);
}

bool is_valid_day2_part1_pw(const std::pair<corporate_policy, std::string>& pw)
{
    auto c = std::ranges::count(pw.second, pw.first.target);
    return (c >= pw.first.min_count && c <= pw.first.max_count);
}

bool is_valid_day2_part2_pw(const std::pair<corporate_policy, std::string>& pw)
{
    const auto& str    = pw.second;
    int         pos1   = pw.first.min_count - 1;
    int         pos2   = pw.first.max_count - 1;
    char        target = pw.first.target;

    return (str[pos1] == target && str[pos2] != target)
           || (str[pos1] != target && str[pos2] == target);
}

int calculate_seat_id(std::string_view pass)
{
    auto to_bits = [](char c) {
        if (c == 'F' || c == 'L')
            return '0';
        else
            return '1';
    };

    return std::bitset<10>(pass | views::transform(to_bits) | to<std::string>).to_ulong();
}

} // namespace aoc2020
