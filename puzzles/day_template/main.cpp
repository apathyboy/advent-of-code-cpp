
#ifndef UNIT_TESTING

int main()
{
    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

TEST_CASE("Can solve part 1 example") {}

TEST_CASE("Can solve part 2 example") {}

#endif