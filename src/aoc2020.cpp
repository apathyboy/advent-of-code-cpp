
#include "aoc2020.hpp"

#include <algorithm>
#include <iostream>
#include <ranges>

namespace aoc2020 {

std::vector<int> read_int_per_line(std::istream& input)
{
    std::vector<int> result;
    int              tmp;

    while (input >> tmp) {
        result.push_back(tmp);
    }

    return result;
}

std::vector<std::pair<int, int>> find_sum_to_pairs(const std::vector<int>& vals, int target)
{
    std::vector<std::pair<int, int>> results;

    for (auto n : vals) {
        for (auto m : vals) {
            if (n == m)
                continue;

            if (n + m == target) {
                results.push_back(std::make_pair(n, m));
            }
        }
    }

    return results;
}

std::vector<std::tuple<int, int, int>>
find_sum_to_triplets(const std::vector<int>& vals, int target)
{
    std::vector<std::tuple<int, int, int>> results;

    for (auto n : vals) {
        for (auto m : vals) {
            if (n == m)
                continue;

            for (auto l : vals) {
                if (n == l || m == l)
                    continue;

                if (n + m + l == target) {
                    results.push_back(std::tie(n, m, l));
                }
            }
        }
    }

    return results;
}

} // namespace aoc2020
