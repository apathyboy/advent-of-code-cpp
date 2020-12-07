
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <set>
#include <string>

using namespace ranges;

int main()
{
    {
        std::ifstream  ifs{"days/day06/input.txt"};
        std::string    str;
        std::set<char> q1;
        size_t         part1_sum = 0;

        while (std::getline(ifs, str)) {
            if (str.length() == 0) {
                part1_sum += q1.size();
                q1.clear();
            }

            insert(q1, str);
        }

        fmt::print("Part 1 Solution: {}\n", part1_sum);
    }

    {
        std::ifstream ifs{"days/day06/input.txt"};

        int part2_sum = 0;

        std::string str;
        std::string questions;

        std::getline(ifs, questions);
        sort(questions);

        while (std::getline(ifs, str)) {
            sort(str);

            if (str.length() == 0) {
                part2_sum += static_cast<int>(questions.size());

                std::getline(ifs, questions);
                sort(questions);
            }
            else {
                questions = views::set_intersection(questions, str) | to<std::string>;
            }
        }

        fmt::print("Part 2 Solution: {}\n", part2_sum);
    }

    return 0;
}
