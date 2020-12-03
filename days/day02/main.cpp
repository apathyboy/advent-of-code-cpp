
#include <aoc2020/aoc2020.hpp>

#include <fstream>
#include <iostream>

int main(int argc, const char** argv)
{
    if (argc != 2) {
        std::cout << "USAGE: day02 <input_file_path>\n" << std::endl;
    }

    std::ifstream ifs{argv[1]};

    return 0;
}
