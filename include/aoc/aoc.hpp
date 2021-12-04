
#pragma once

#include <iosfwd>
#include <vector>

#include <range/v3/all.hpp>

namespace aoc {

template <typename T>
std::vector<T> read_element_per_line(std::istream&& input)
{
    return ranges::istream_view<T>(input) | ranges::to<std::vector>;
}

std::vector<int> read_ints(std::istream&& input, char delimiter);

} // namespace aoc
