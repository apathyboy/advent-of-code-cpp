
#pragma once

#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

namespace aoc2020 {

int calculate_row_id(std::string_view boarding_pass);

int calculate_column(std::string_view boarding_pass);

int calculate_seat_id(std::string_view boarding_pass);

} // namespace aoc2020
