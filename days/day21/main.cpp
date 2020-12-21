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

std::optional<food>
find_single_allergen_food(const std::vector<food>& food_list, std::string search_allergen)
{
    auto find_iter = rs::find_if(food_list, [&search_allergen](const auto& f) {
        return f.allergens.size() == 1 && f.allergens[0] == search_allergen;
    });

    if (find_iter == rs::end(food_list)) { return {}; }

    return *find_iter;
}

std::vector<food>
find_food_with_allergen(const std::vector<food>& food_list, const std::string& search_allergen)
{
    return food_list | rv::filter([&search_allergen](const auto& f) {
               return rs::contains(f.allergens, search_allergen);
           })
           | rs::to_vector;
}

auto create_allergen_map(const std::vector<food>& food_list)
{
    auto unique_allergens = food_list | rv::transform([](auto&& f) { return f.allergens; }) | ra::join
                            | ra::sort | ra::unique;

    return unique_allergens
           | rv::transform([](auto&& s) { return std::make_pair(s, std::optional<std::string>{}); })
           | rs::to<std::map<std::string, std::optional<std::string>>>;
}

auto count_all_non_allergen_ingredients(
    const std::vector<food>&                       food_list,
    const std::vector<std::optional<std::string>>& with_allergens)
{
    auto all_ingredients = food_list | rv::transform([](auto&& f) { return f.ingredients; }) | ra::join
                           | ra::sort;

    auto no_allergens = all_ingredients | rv::filter([&with_allergens](auto&& i) {
                            return rs::none_of(with_allergens, [&i](auto ai) {
                                return ai.value() == i;
                            });
                        });

    return rs::distance(no_allergens);
}

int64_t part1_2(std::vector<food> food_list)
{
    auto allergens = create_allergen_map(food_list);

    while (rs::any_of(allergens, [](auto&& p) { return !p.second; })) {
        for (auto& check_food : food_list) {
            for (auto allergen : check_food.allergens) {
                if (allergens.at(allergen).has_value()) continue;
                auto food_with_allergen = find_food_with_allergen(food_list, allergen);

                for (auto& allergen_food : food_with_allergen) {
                    auto allergen_food_ingredients = rv::set_difference(
                        allergen_food.ingredients,
                        allergens | rv::values | rv::filter([](const auto& p) { return p.has_value(); })
                            | rv::transform([](auto&& p) { return p.value(); }));

                    auto match = rv::set_intersection(check_food.ingredients, allergen_food_ingredients);

                    if (rs::distance(match) == 1) {
                        allergens[allergen] = rs::front(match);
                        break;
                    }
                }
            }
        }
    }

    return count_all_non_allergen_ingredients(food_list, allergens | rv::values | rs::to_vector);
}

int64_t part1(std::vector<food> food_list)
{
    std::map<std::string, std::string> known_allergens;

    for (auto check_food : food_list | rv::filter([](auto&& f) { return f.allergens.size() > 1; })) {
        for (auto check_allergen : check_food.allergens) {
            auto allergen_food = find_single_allergen_food(food_list, check_allergen);

            if (!allergen_food) continue;

            auto allergen_food_ingredients = rv::set_difference(
                allergen_food->ingredients,
                known_allergens | rv::values);

            auto match = rv::set_intersection(check_food.ingredients, allergen_food_ingredients);

            if (rs::distance(match) == 1) {

                auto ingredient = rs::front(match);

                known_allergens.insert(std::make_pair(check_allergen, ingredient));
            }
        }
    }

    for (auto check_food : food_list) {
        auto unknown_allergens = rv::set_difference(check_food.allergens, known_allergens | rv::keys);

        if (rs::distance(unknown_allergens) == 0) continue;

        auto remaining_ingredients = rv::set_difference(
            check_food.ingredients,
            known_allergens | rv::values);

        if (rs::distance(remaining_ingredients) == 1 && rs::distance(unknown_allergens) == 1) {
            known_allergens.insert(
                std::make_pair(rs::front(unknown_allergens), rs::front(remaining_ingredients)));
        }
    }

    auto all_ingredients = food_list | rv::transform([](auto&& f) { return f.ingredients; }) | ra::join
                           | ra::sort;

    auto all_allergen_ingredients = known_allergens | rv::transform([](auto& p) { return p.second; })
                                    | rs::to_vector | ra::sort;

    auto no_allergens = all_ingredients | rv::filter([&all_allergen_ingredients](auto&& i) {
                            return rs::none_of(all_allergen_ingredients, [&i](auto ai) {
                                return ai == i;
                            });
                        })
                        | rs::to_vector;

    return rs::distance(no_allergens);
}

int64_t part2()
{
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2020 - Day 21\n");

    std::string input_path = "days/day21/puzzle.in";

    auto food_list = read_input(std::ifstream{input_path});

    fmt::print("Part 1 Solution: {}\n", part1_2(food_list));
    fmt::print("Part 2 Solution: {}\n", part2());

    return 0;
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
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

    ss << R"()";

    REQUIRE(0 == part2());
}

#endif
