
#include <fmt/core.h>

#include <fstream>
#include <set>
#include <string>

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day06 <input_file_path>\n");
    }

    std::ifstream ifs{argv[1]};

    int sum_of_counts = 0;

    std::set<char> questions;

    std::string str;
    while (std::getline(ifs, str)) {
        if (str.length() == 0) {
            sum_of_counts += static_cast<int>(questions.size());
            questions.clear();
        }

        questions.insert(str.begin(), str.end());
    }

    fmt::print("Part 1 Solution: {}\n", sum_of_counts);

    return 0;
}
