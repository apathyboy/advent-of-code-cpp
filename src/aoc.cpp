
#include "aoc.hpp"

#include <range/v3/all.hpp>

namespace rs = ranges;
namespace rv = ranges::views;

namespace aoc {

std::vector<int> read_ints(std::istream&& input, char delimiter)
{
    // clang-format off
    return rs::getlines(input, delimiter) 
        | rv::transform([](auto&& s) { return std::stoi(s); })
        | rs::to<std::vector>;
    // clang-format on
}

} // namespace aoc
