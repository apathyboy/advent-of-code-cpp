
#pragma once

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace aoc2020 {

std::vector<int> read_int_per_line(std::istream& input);

std::vector<std::pair<int, int>> find_sum_to_pairs(const std::vector<int>& vals, int target);

std::vector<std::tuple<int, int, int>>
find_sum_to_triplets(const std::vector<int>& vals, int target);

int calculate_row_id(std::string_view boarding_pass);

int calculate_column(std::string_view boarding_pass);

int calculate_seat_id(std::string_view boarding_pass);

} // namespace aoc2020
