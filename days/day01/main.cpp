
#include <aoc2020/aoc2020.hpp>

#include <fstream>
#include <iostream>

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
        std::cout << "USAGE: day01 <input_file_path>\n" << std::endl;
    }

    std::ifstream ifs{argv[1]};

    auto nums = aoc2020::read_int_per_line(ifs);

    std::cout << "Part 1 Solution: " << day1_part1(nums) << std::endl;

    std::cout << "Part 2 Solution: " << day1_part2(nums) << std::endl;

    return 0;
}
