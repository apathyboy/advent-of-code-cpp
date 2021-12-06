
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

template <typename T>
std::vector<T> split_line_by(std::istream& input, char delimiter = ',')
{
    std::string tmp;
    std::getline(input, tmp);

    // clang-format off
    return tmp | ranges::views::split(delimiter)
               | ranges::views::transform([](auto&& s) { return s | ranges::to<std::string>; })
               | ranges::views::transform([](auto&& s) { return std::stoi(s); })
               | ranges::to<std::vector>;
    // clang-format on
}


template <typename T>
auto transpose(const std::vector<std::vector<T>>& grid)
{
    // clang-format off
    return ranges::views::iota(0, static_cast<int>(grid[0].size())) 
        | ranges::views::transform([&grid](int i) {
            return grid 
                | ranges::views::transform([i](auto&& t) { return t[i]; }) 
                | ranges::views::reverse 
                | ranges::to_vector; })
        | ranges::to_vector;
    // clang-format on
}

} // namespace aoc
