#include <fmt/format.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <set>

namespace rs = ranges;
namespace ra = ranges::actions;
namespace rv = ranges::views;

struct food {
    std::vector<std::string> ingredients;
    std::vector<std::string> allergens;
};

std::vector<food> read_input(std::istream&& input)
{
    return rs::getlines(input) | rv::transform([](auto&& line) {
               std::smatch m;
               if (!std::regex_match(line, m, std::regex(R"((.*)\(contains (.*)\))"))) {
                   throw std::runtime_error{"Invalid input found"};
               }

               auto ingredients_tmp = m.str(1);
               auto allergens_tmp   = m.str(2);

               food f;

               f.ingredients = ingredients_tmp | rv::split(' ')
                               | rv::transform([](auto&& rng) { return rng | rs::to<std::string>; })
                               | rs::to_vector | ra::sort;

               f.allergens = allergens_tmp | rv::split(',')
                             | rv::transform([](auto&& rng) { return rng | rs::to<std::string>; })
                             | rv::transform([](auto&& s) {
                                   return s | rv::trim([](uint8_t c) { return std::isspace(c); })
                                          | rs::to<std::string>;
                               })
                             | rs::to_vector | ra::sort;

               return f;
           })
           | rs::to_vector;
}

std::vector<food>
find_food_with_allergen(const std::vector<food>& food_list, const std::string& search_allergen)
{
    return food_list | rv::filter([&search_allergen](const auto& f) {
               return rs::contains(f.allergens, search_allergen);
           })
           | rs::to_vector;
}

int64_t count_all_non_allergen_ingredients(
    const std::vector<food>&        food_list,
    const std::vector<std::string>& with_allergens)
{
    auto all_ingredients = food_list | rv::transform([](auto&& f) { return f.ingredients; }) | ra::join;

    auto no_allergens = all_ingredients | rv::filter([&with_allergens](auto&& i) {
                            return rs::none_of(with_allergens, [&i](auto ai) { return ai == i; });
                        });

    return rs::distance(no_allergens);
}

auto build_allergen_map(const std::vector<food>& food_list)
{
    std::map<std::string, std::set<std::string>> allergen_potentials;

    for (auto& check_food : food_list) {
        for (auto allergen : check_food.allergens) {
            auto food_with_allergen = find_food_with_allergen(food_list, allergen);

            auto potentials = check_food.ingredients;

            for (const auto& allergen_food : food_with_allergen) {
                potentials = rv::set_intersection(potentials, allergen_food.ingredients) | rs::to_vector;
            }

            for (auto p : potentials) {
                allergen_potentials[allergen].insert(p);
            }
        }
    }

    for (auto& allergen : allergen_potentials) {
        for (auto& test_allergen : allergen_potentials) {
            if (test_allergen.first == allergen.first) continue;
            if (allergen.second.size() == 1) break;

            allergen.second = rv::set_difference(allergen.second, test_allergen.second)
                              | rs::to<std::set<std::string>>;
        }
    }

    return allergen_potentials | rv::transform([](auto&& p) { return rs::front(p.second); })
           | rs::to_vector;
}

int64_t part1(std::vector<food> food_list)
{
    return count_all_non_allergen_ingredients(food_list, build_allergen_map(food_list));
}

std::string part2(std::vector<food> food_list)
{
    auto allergens = build_allergen_map(food_list);
    return allergens | rv::join(',') | rs::to<std::string>;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 21\n");

    std::string input_path = "puzzle.in";

    auto food_list = read_input(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1(food_list));
    fmt::print("Part 2 Solution: {}\n", part2(food_list));

    return 0;
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can read input")
{
    std::stringstream ss;

    ss << R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish))";

    auto food_list = read_input(std::move(ss));

    REQUIRE(4 == food_list.size());
    REQUIRE(std::vector{std::string{"fvjkl"}, std::string{"sqjhc"}} == food_list[2].ingredients);
    REQUIRE(std::vector{std::string{"dairy"}, std::string{"fish"}} == food_list[0].allergens);
}

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;

    ss << R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish))";

    auto food_list = read_input(std::move(ss));

    REQUIRE(5 == part1(food_list));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;

    ss << R"(mxmxvkd kfcds sqjhc nhms (contains dairy, fish)
trh fvjkl sbzzf mxmxvkd (contains dairy)
sqjhc fvjkl (contains soy)
sqjhc mxmxvkd sbzzf (contains fish))";

    auto food_list = read_input(std::move(ss));

    REQUIRE(std::string{"mxmxvkd,sqjhc,fvjkl"} == part2(food_list));
}

#endif
