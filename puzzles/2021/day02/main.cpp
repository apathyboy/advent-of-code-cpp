
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <ranges>
#include <sstream>
#include <string>

#ifndef UNIT_TESTING

int main()
{
    std::ifstream in{"puzzle.in"};

    auto t = std::ranges::istream_view<int>(in) | std::ranges::views::take(3);

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Can solve part 1 example") {}

TEST_CASE("Can solve part 2 example") {}

#endif