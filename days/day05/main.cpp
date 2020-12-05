
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day05 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    auto seat_ids = ranges::getlines(ifs) | ranges::views::transform([](const auto& s) {
                        return aoc2020::calculate_seat_id(s);
                    })
                    | ranges::to<std::vector<int>>;

    auto [min, max] = ranges::minmax(seat_ids);
    auto sum        = ranges::accumulate(seat_ids, 0);

    fmt::print("Part 1 Solution: {}\n", max);
    fmt::print("Part 2 Solution: {}\n", ((seat_ids.size() + 1) * (min + max) / 2) - sum);

    return 0;
}
