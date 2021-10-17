
#include "aoc.hpp"

#include <range/v3/all.hpp>

namespace rs = ranges;
namespace rv = ranges::views;

namespace aoc {

std::vector<int> read_int_per_line(std::istream&& input)
{
    return rs::getlines(input) | rv::transform([](auto&& s) { return std::stoi(s); })
           | rs::to<std::vector>;
}

} // namespace aoc
