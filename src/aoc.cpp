
#include "aoc.hpp"

#include <range/v3/all.hpp>

namespace rs = ranges;
namespace rv = ranges::views;

namespace aoc {

std::vector<int> read_int_per_line(std::istream&& input)
{
    // clang-format off
    return rs::istream_view<int>(input) | rs::to<std::vector>;
    // clang-format on
}

std::vector<int> read_ints(std::istream&& input, char delimiter)
{
    // clang-format off
    return rs::getlines(input, delimiter) 
        | rv::transform([](auto&& s) { return std::stoi(s); })
        | rs::to<std::vector>;
    // clang-format on
}

} // namespace aoc
