
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>

#pragma warning(push)
#pragma warning(disable : 28278)
#include <range/v3/all.hpp>
#pragma warning(pop)

#include <fstream>

using namespace aoc2020;
using namespace ranges;

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day05 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    auto seat_ids = getlines(ifs) | views::transform(calculate_seat_id) | to<std::vector<int>>;

    auto [min_id, max_id] = minmax(seat_ids);
    auto sum              = accumulate(seat_ids, 0);

    fmt::print("Part 1 Solution: {}\n", max_id);
    fmt::print("Part 2 Solution: {}\n", ((seat_ids.size() + 1) * (min_id + max_id) / 2) - sum);

    return 0;
}
