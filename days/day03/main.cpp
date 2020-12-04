
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <string>

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day02 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    std::string str;
    int         slope1 = 1, slope2 = 3, slope3 = 5, slope4 = 7;
    int         count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;
    int         depth = 0;

    while (std::getline(ifs, str)) {
        if (str[(depth * slope1) % str.length()] == '#')
            ++count1;

        if (str[(depth * slope2) % str.length()] == '#')
            ++count2;

        if (str[(depth * slope3) % str.length()] == '#')
            ++count3;

        if (str[(depth * slope4) % str.length()] == '#')
            ++count4;

        if (depth % 2 == 0 && str[(depth / 2) % str.length()] == '#')
            ++count5;

        ++depth;
    }

    fmt::print("Part 1 Solution: {}\n", count2);
    fmt::print("Part 2 Solution: {}\n", count1 * count2 * count3 * count4 * count5);

    return 0;
}
