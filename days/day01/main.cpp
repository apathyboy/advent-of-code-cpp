#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <array>
#include <fstream>

namespace rv = ranges::views;

auto contains_last_element(const std::vector<int>& input)
{
    return [&input](const auto& p) { return ranges::contains(input, p.back()); };
}

auto accumulate_multiply()
{
    return [](auto&& p) { return ranges::accumulate(p, 1, std::multiplies<>()); };
}

int part1(const std::vector<int>& input)
{
    // clang-format off
    auto result = input 
        | rv::transform([](auto i) { return std::array{i, 2020 - i}; })
        | rv::filter(contains_last_element(input))
        | rv::transform(accumulate_multiply());
    // clang-format on

    return ranges::front(result);
}

int part2(const std::vector<int>& input)
{
    // clang-format off
    auto result = rv::cartesian_product(input, input)
        | rv::transform([](auto&& i) { 
            auto [a, b] = i;
            return std::array{a, b, 2020 - a - b}; })
        | rv::filter(contains_last_element(input))
        | rv::transform(accumulate_multiply());
    // clang-format on

    return ranges::front(result);
}

int main()
{
    std::ifstream ifs{"days/day01/input.txt"};

    std::vector<int> input = ranges::getlines(ifs)
                             | rv::transform([](auto&& s) { return std::stoi(s); })
                             | ranges::to<std::vector<int>>;

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));

    return 0;
}
