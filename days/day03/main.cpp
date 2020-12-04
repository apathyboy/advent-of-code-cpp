
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
    int         pos     = 0;
    int         counter = 0;

    while (std::getline(ifs, str)) {
        if (str[pos % str.length()] == '#')
            ++counter;

        pos += 3;
    }

    fmt::print("Part 1 Solution: {}\n", counter);

    return 0;
}
