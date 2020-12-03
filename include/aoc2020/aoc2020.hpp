
#pragma once

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace aoc2020 {

struct corporate_policy {
    int  min_count;
    int  max_count;
    char target;
};

std::vector<int> read_int_per_line(std::istream& input);

std::vector<std::pair<int, int>> find_sum_to_pairs(const std::vector<int>& vals, int target);

std::vector<std::tuple<int, int, int>>
find_sum_to_triplets(const std::vector<int>& vals, int target);

std::pair<corporate_policy, std::string> parse_password_rule_string(const std::string& str);

bool is_valid_day2_part1_pw(const std::pair<corporate_policy, std::string>& pw);

bool is_valid_day2_part2_pw(const std::pair<corporate_policy, std::string>& pw);

} // namespace aoc2020
