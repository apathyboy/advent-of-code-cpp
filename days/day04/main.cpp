#include <boost/algorithm/string.hpp>
#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <ranges>
#include <set>
#include <string>
#include <sstream>
#include <vector>

using namespace ranges;

struct field_t {
    std::string tag;
    std::string data;
};

using passport = std::vector<field_t>;

int main(int argc, const char** argv)
{
    if (argc != 2) {
        fmt::print("USAGE: day04 <input_file_path>\n");
    }

    {
        std::ifstream ifs{argv[1]};

        std::string str;

        int valid_passports = 0;

        std::set<std::string> check;

        while (std::getline(ifs, str)) {
            if (str.length() == 0) {
                if (check.size() == 7)
                    ++valid_passports;

                check.clear();
            }
            else {
                auto fields = str | views::split(' ') | views::transform([](auto const& rc) {
                                  return rc | to<std::string>;
                              })
                              | views::transform([](const auto& s) { return s.substr(0, 3); })
                              | views::filter([](const auto& s) { return s != "cid"; })
                              | views::unique;

                for (const auto& s : fields) {
                    check.insert(s);
                }
            }
        }

        fmt::print("Part 1 Solution: {}\n", valid_passports);
    }

    {
        std::ifstream ifs{argv[1]};

        std::string str;

        int valid_passports = 0;

        std::vector<field_t> check;

        while (std::getline(ifs, str)) {
            if (str.length() == 0) {
                if (check.size() == 7)
                    ++valid_passports;

                check.clear();
            }
            else {
                auto fields =
                    str | views::split(' ')
                    | views::transform([](auto const& rc) { return rc | to<std::string>; })
                    | views::transform([](const auto& s) {
                          return field_t{s.substr(0, 3), s.substr(4)};
                      })
                    | views::filter([](const auto& f) { return f.tag != "cid"; })
                    | views::filter([](const auto& f) {
                          if (f.tag == "byr") {
                              int d = std::stoi(f.data);
                              return d >= 1920 && d <= 2002;
                          }
                          else if (f.tag == "iyr") {
                              int d = std::stoi(f.data);
                              return d >= 2010 && d <= 2020;
                          }
                          else if (f.tag == "eyr") {
                              int d = std::stoi(f.data);
                              return d >= 2020 && d <= 2030;
                          }
                          else if (f.tag == "hgt") {

                              std::string unit = f.data.substr(f.data.length() - 2);
                              if (unit != "in" && unit != "cm")
                                  return false;

                              int h = std::stoi(f.data.substr(0, f.data.length() - 2));
                              return (unit == "cm" && h >= 150 && h <= 193)
                                     || (unit == "in" && h >= 59 && h <= 76);
                          }
                          else if (f.tag == "hcl") {
                              if (f.data.length() != 7)
                                  return false;

                              if (f.data[0] != '#')
                                  return false;

                              for (auto c : f.data.substr(1)) {
                                  if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f'))
                                      return false;
                              }
                          }
                          else if (f.tag == "ecl") {
                              return f.data == "amb" || f.data == "blu" || f.data == "brn"
                                     || f.data == "gry" || f.data == "grn" || f.data == "hzl"
                                     || f.data == "oth";
                          }
                          else if (f.tag == "pid") {
                              if (f.data.length() != 9)
                                  return false;

                              for (auto c : f.data) {
                                  if (c < '0' || c > '9')
                                      return false;
                              }
                          }

                          return true;
                      });

                for (const auto& s : fields) {
                    check.push_back(s);
                }
            }
        }

        fmt::print("Part 2 Solution: {}\n", valid_passports);
    }

    return 0;
}
