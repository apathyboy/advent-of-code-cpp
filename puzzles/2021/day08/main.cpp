#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct calibration_t {
    std::vector<std::string> signal_patterns;
    std::vector<std::string> digits;
};

auto combine_chars = [](auto const& rc) { return rc | rs::to<std::string>; };

char position_a(const std::vector<std::string>& signal_patterns)
{
    auto r = signal_patterns
             | rv::filter([](const auto& p) { return p.length() == 2 || p.length() == 3; });

    auto r2 = rv::set_difference(r.front(), r.back());

    return rs::front(r2);
}


int solve(const calibration_t& c)
{
    char a = position_a(c.signal_patterns);
    (a);
    return 0;
}

calibration_t read_calibration(const std::string& input)
{
    calibration_t t{};
    bool          read_digits = false;

    for (std::string item : input | rv::split(' ') | rv::transform(combine_chars)) {
        if (item == "|") { read_digits = true; }

        if (read_digits) { t.digits.push_back(item); }
        else {
            t.signal_patterns.push_back(item);
        }
    }

    return t;
}

std::vector<calibration_t> read_input(std::istream&& input)
{
    return rs::getlines(input) | rv::transform(read_calibration) | rs::to<std::vector>;
}

int part1(const std::vector<calibration_t>& input)
{
    int counter = 0;

    for (auto c : input) {
        for (auto d : c.digits) {
            if (d.length() == 2 || d.length() == 3 || d.length() == 4 || d.length() == 7) { ++counter; }
        }
    }

    return counter;
}

int part2(const std::vector<calibration_t>& input)
{
    (input);
    return 0;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 08\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(std::move(ifs));

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce)";

    auto input = read_input(std::move(ss));

    REQUIRE(26 == part1(input));
}

TEST_CASE("Can solve puzzle line")
{
    auto c = read_calibration(
        "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf");

    REQUIRE(5353 == solve(c));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | fdgacbe cefdb cefbgd gcbe
edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | fcgedb cgb dgebacf gc
fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | cg cg fdcagb cbg
fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | efabcd cedba gadfec cb
aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | gecf egdcabf bgf bfgea
fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | gebdcfa ecba ca fadegcb
dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | cefg dcbef fcge gbcadfe
bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | ed bcgafe cdgba cbgef
egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | gbdfcae bgc cg cgb
gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | fgae cfgab fg bagce)";

    auto input = read_input(std::move(ss));

    REQUIRE(0 == part2(input));
    // REQUIRE(61229 == part2(input));
}

#endif