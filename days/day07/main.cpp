
#include <fmt/core.h>

#include <fstream>
#include <string>

int main()
{
    fmt::print("Advent of Code 2020 - Day 07\n");

    std::ifstream ifs{"days/day07/input.txt"};

    std::string str;
    std::getline(ifs, str);

    return 0;
}
