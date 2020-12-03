
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>

#include <fstream>

int day1_part1(const std::vector<int>& nums)
{
    auto vals = aoc2020::find_sum_to_pairs(nums, 2020);

    return vals[0].first * vals[0].second;
}

int day1_part2(const std::vector<int>& nums)
{
    auto vals = aoc2020::find_sum_to_triplets(nums, 2020);

    int val1, val2, val3;

    std::tie(val1, val2, val3) = vals[0];

    return val1 * val2 * val3;
}

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day01 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    auto input = aoc2020::read_int_per_line(ifs);

    fmt::print("Part 1 Solution: {}", day1_part1(input));
    fmt::print("Part 2 Solution: ", day1_part2(input));

    return 0;
}
