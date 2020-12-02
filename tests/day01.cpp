
#include <aoc2020/aoc2020.hpp>

#include <catch2/catch.hpp>

TEST_CASE("Can find two numbers that equal sum in list")
{
    std::vector<int> nums = {1721, 979, 366, 299, 675, 1456};

    auto results = aoc2020::find_sum_to_pairs(nums, 2020);

    REQUIRE(results[0].first == 1721);
    REQUIRE(results[0].second == 299);
}

TEST_CASE("Can find three numbers that equal sum in list")
{
    std::vector<int> nums = {1721, 979, 366, 299, 675, 1456};

    auto results = aoc2020::find_sum_to_triplets(nums, 2020);

    int val1, val2, val3;

    std::tie(val1, val2, val3) = results[0];

    REQUIRE(val1 == 979);
    REQUIRE(val2 == 366);
    REQUIRE(val3 == 675);
}
