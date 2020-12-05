#include <fmt/core.h>

#include <fstream>

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day04 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    return 0;
}