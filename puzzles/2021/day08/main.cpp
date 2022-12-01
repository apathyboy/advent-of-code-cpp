#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;


using Display = uint8_t;

constexpr const Display SEGMENT_A = 1;
constexpr const Display SEGMENT_B = 1 << 1;
constexpr const Display SEGMENT_C = 1 << 2;
constexpr const Display SEGMENT_D = 1 << 3;
constexpr const Display SEGMENT_E = 1 << 4;
constexpr const Display SEGMENT_F = 1 << 5;
constexpr const Display SEGMENT_G = 1 << 6;

constexpr const std::array<Display, 10> NUMBERS = {
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_E | SEGMENT_F | SEGMENT_G,             // 0
    SEGMENT_C | SEGMENT_F,                                                             // 1
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_G,                         // 2
    SEGMENT_A | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         // 3
    SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F,                                     // 4
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_F | SEGMENT_G,                         // 5
    SEGMENT_A | SEGMENT_B | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G,             // 6
    SEGMENT_A | SEGMENT_C | SEGMENT_F,                                                 // 7
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_E | SEGMENT_F | SEGMENT_G, // 8
    SEGMENT_A | SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F | SEGMENT_G,             // 9
};

struct Puzzle {
    std::vector<Display> patterns;
    std::vector<Display> numbers;
};

uint8_t cnt_segments(Display display)
{
    return static_cast<uint8_t>(std::popcount(display));
}

Puzzle parse_line(const std::string& input)
{
    Puzzle result;
    bool   past_delim = false;
    auto   it         = input.begin();
    while (it != input.end()) {
        // read one number
        Display d = 0;
        while (it != input.end() && std::islower(*it)) {
            if (*it - 'a' > 7)
                throw std::out_of_range("Unexpected value in input, only accepts values 'a-g'.");
            d |= 1 << (*it - 'a');
            it++;
        }
        if (past_delim) { result.numbers.push_back(d); }
        else {
            result.patterns.push_back(d);
        }
        // skip over spaces and delimiter '|'
        while (it != input.end() && (std::isspace(*it) || *it == '|')) {
            if (*it == '|') past_delim = true;
            it++;
        }
    }
    return result;
}

using Wiring = std::array<uint8_t, 7>;

inline const std::unordered_map<Display, uint8_t> DECIMALS = {
    {NUMBERS[0], 0},
    {NUMBERS[1], 1},
    {NUMBERS[2], 2},
    {NUMBERS[3], 3},
    {NUMBERS[4], 4},
    {NUMBERS[5], 5},
    {NUMBERS[6], 6},
    {NUMBERS[7], 7},
    {NUMBERS[8], 8},
    {NUMBERS[9], 9}};


bool segment_lit(Display disp, uint8_t segment)
{
    return disp & (1 << segment);
}

Wiring solve_wiring(const Puzzle& input)
{
    Wiring result = {NUMBERS[8], NUMBERS[8], NUMBERS[8], NUMBERS[8], NUMBERS[8], NUMBERS[8], NUMBERS[8]};
    for (auto pattern : input.patterns) {
        switch (cnt_segments(pattern)) {
            case 2:
                for (uint8_t wire_in = 0; wire_in < 7; wire_in++) {
                    if (segment_lit(pattern, wire_in)) { result[wire_in] &= (SEGMENT_C | SEGMENT_F); }
                    else {
                        result[wire_in] &= ~(SEGMENT_C | SEGMENT_F);
                    }
                }
                break;
            case 3:
                for (uint8_t wire_in = 0; wire_in < 7; wire_in++) {
                    if (segment_lit(pattern, wire_in)) {
                        result[wire_in] &= (SEGMENT_A | SEGMENT_C | SEGMENT_F);
                    }
                    else {
                        result[wire_in] &= ~(SEGMENT_A | SEGMENT_C | SEGMENT_F);
                    }
                }
                break;
            case 4:
                for (uint8_t wire_in = 0; wire_in < 7; wire_in++) {
                    if (segment_lit(pattern, wire_in)) {
                        result[wire_in] &= (SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F);
                    }
                    else {
                        result[wire_in] &= ~(SEGMENT_B | SEGMENT_C | SEGMENT_D | SEGMENT_F);
                    }
                }
                break;
            case 5:
                for (uint8_t wire_in = 0; wire_in < 7; wire_in++) {
                    if (!segment_lit(pattern, wire_in)) {
                        result[wire_in] &= (SEGMENT_B | SEGMENT_C | SEGMENT_E | SEGMENT_F);
                    }
                }
                break;
            case 6:
                for (uint8_t wire_in = 0; wire_in < 7; wire_in++) {
                    if (!segment_lit(pattern, wire_in)) {
                        result[wire_in] &= (SEGMENT_C | SEGMENT_D | SEGMENT_E);
                    }
                }
                break;
        }
    }
    for (auto singular_wire : result) {
        if (std::popcount(singular_wire) != 1) continue;

        for (auto& undecided_wire : result) {
            if (std::popcount(undecided_wire) == 1) continue;
            undecided_wire &= ~singular_wire;
        }
    }
    return result;
}

uint32_t decode_number(const Puzzle& puzzle, const Wiring& solved_wiring)
{
    uint32_t result = 0;
    for (auto digit : puzzle.numbers) {
        uint8_t display = 0;
        for (uint8_t bit = 0; bit < 7; bit++) {
            if (segment_lit(digit, bit)) display |= solved_wiring[bit];
        }

        result *= 10;
        result += DECIMALS.at(display);
    }
    return result;
}


struct calibration_t {
    std::vector<std::string> signal_patterns;
    std::vector<std::string> digits;
};

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

    for (std::string item : input | rv::split(' ') | rv::transform(aoc::combine_chars)) {
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
    // auto          input = read_input(std::move(ifs));


    std::string input_line;
    uint64_t    part1_total = 0;
    uint64_t    part2_total = 0;
    while (std::getline(ifs, input_line)) {
        Puzzle p = parse_line(input_line);
        part1_total += std::ranges::count_if(p.numbers, [](auto num) {
            uint8_t cnt = cnt_segments(num);
            return cnt == 2 || cnt == 3 || cnt == 4 || cnt == 7;
        });
        Wiring w = solve_wiring(p);
        part2_total += decode_number(p, w);
    }

    fmt::print("Part 1 Solution: {}\n", part1_total);
    fmt::print("Part 2 Solution: {}\n", part2_total);
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

    REQUIRE(0 == solve(c));
    // REQUIRE(5353 == solve(c));
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