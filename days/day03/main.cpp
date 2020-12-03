
#include <aoc2020/aoc2020.hpp>

#include <fmt/core.h>
#include <fstream>

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day02 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    return 0;
}
