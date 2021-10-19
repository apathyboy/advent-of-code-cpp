
#pragma once

#include <iosfwd>
#include <vector>

namespace aoc {

std::vector<int> read_int_per_line(std::istream&& input);
std::vector<int> read_ints(std::istream&& input, char delimiter);

} // namespace aoc
