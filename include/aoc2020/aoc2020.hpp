
#pragma once

#include <iosfwd>
#include <utility>
#include <vector>

namespace aoc2020 {

std::vector<int> read_int_per_line(std::istream& input);

std::vector<std::pair<int, int>> find_sum_to_pairs(const std::vector<int>& vals, int target);
std::vector<std::tuple<int, int, int>>
find_sum_to_triplets(const std::vector<int>& vals, int target);

} // namespace aoc2020
