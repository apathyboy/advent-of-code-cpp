
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
