#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

using path_t = std::vector<std::string>;

using transition_t = std::pair<std::string, std::string>;

std::vector<transition_t> read_input(std::istream& input)
{
    return rs::getlines(input) | rv::transform([](auto&& s) {
               std::string left  = s.substr(0, s.find_first_of('-'));
               std::string right = s.substr(s.find_first_of('-') + 1);

               if (right == "start" || left == "end") { std::swap(left, right); }

               return std::pair{left, right};
           })
           | rs::to<std::vector>;
}

bool contains(const std::vector<std::string>& c, const std::string& t)
{
    return rs::find(c, t) != c.end();
}

std::vector<transition_t> get_transitions_from(
    const std::vector<transition_t>& transitions,
    std::string                      cave,
    const std::vector<std::string>&  visited)
{
    auto small_visited = visited | rv::drop(1) | rv::filter([](const auto& t) { return islower(t[0]); })
                         | rs::to<std::vector>;

    return transitions | rv::filter([cave](const auto& t) {
               bool transition_found = t.first == cave || t.second == cave;

               if (t.first == "start") { transition_found = false; }

               return transition_found;
           })
           | rv::transform([cave](auto t) {
                 if (t.second == cave) std::swap(t.first, t.second);
                 return t;
             })
           | rv::filter([&small_visited](const auto& t) { return !contains(small_visited, t.second); })
           | rs::to<std::vector>;
}

void explore(
    const std::vector<transition_t>& transitions,
    path_t&                          path_base,
    std::string                      current_cave,
    std::vector<path_t>&             paths)
{
    for (const auto& next : get_transitions_from(transitions, current_cave, path_base)) {
        path_t path = path_base;

        path.push_back(next.second);

        if (next.second == "end") { paths.push_back(path); }
        else {
            explore(transitions, path, next.second, paths);
        }
    }
}

std::vector<std::string> find_all_smalls(const std::vector<transition_t>& transitions)
{
    std::vector<std::string> smalls;

    for (const auto& t : transitions) {
        if (t.first != "start") {
            if (islower(t.first[0]) && !contains(smalls, t.first)) { smalls.push_back(t.first); }
        }
        if (t.second != "end") {
            if (islower(t.second[0]) && !contains(smalls, t.second)) { smalls.push_back(t.second); }
        }
    }

    return smalls;
}

std::vector<transition_t> get_transitions_from2(
    const std::vector<transition_t>& transitions,
    std::string                      cave,
    const std::vector<std::string>&  visited,
    std::string                      allowed_twice)
{
    auto small_visited = visited | rv::drop(1) | rv::filter([](const auto& t) { return islower(t[0]); })
                         | rs::to<std::vector>;

    return transitions | rv::filter([cave](const auto& t) {
               bool transition_found = t.first == cave || t.second == cave;

               if (t.first == "start") { transition_found = false; }

               return transition_found;
           })
           | rv::transform([cave](auto t) {
                 if (t.second == cave) std::swap(t.first, t.second);
                 return t;
             })
           | rv::filter([&small_visited, visited, allowed_twice](const auto& t) {
                 bool keep = true;
                 if (islower(t.second[0])) {
                     if (t.second == allowed_twice) { keep = rs::count(visited, allowed_twice) < 2; }
                     else {
                         keep = !contains(small_visited, t.second);
                     }
                 }
                 return keep;
             })
           | rs::to<std::vector>;
}

void explore2(
    const std::vector<transition_t>& transitions,
    path_t&                          path_base,
    std::string                      current_cave,
    std::vector<path_t>&             paths,
    std::string                      allowed_twice)
{
    for (const auto& next : get_transitions_from2(transitions, current_cave, path_base, allowed_twice)) {
        path_t path = path_base;

        path.push_back(next.second);

        if (next.second == "end") { paths.push_back(path); }
        else {
            explore2(transitions, path, next.second, paths, allowed_twice);
        }
    }
}

int64_t part1(const std::vector<transition_t>& transitions)
{
    std::vector<path_t> paths;
    for (const auto& start :
         transitions | rv::filter([](const auto& c) { return c.first == "start"; })) {
        path_t p{};
        p.push_back(start.first);
        p.push_back(start.second);

        explore(transitions, p, start.second, paths);
    }

    return paths.size();
}

int64_t part2(const std::vector<transition_t>& transitions)
{
    std::vector<path_t>      paths;
    std::vector<std::string> small_caves = find_all_smalls(transitions);

    for (const auto& start :
         transitions | rv::filter([](const auto& c) { return c.first == "start"; })) {
        path_t p{};
        p.push_back(start.first);
        p.push_back(start.second);

        for (auto s : small_caves) {
            explore2(transitions, p, start.second, paths, s);
        }
    }

    rs::sort(paths);

    paths = paths | rv::unique | rs::to<std::vector>;

    return paths.size();
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 12\n");

    std::ifstream ifs{"puzzle.in"};
    auto          input = read_input(ifs);

    fmt::print("Part 1 Solution: {}\n", part1(input));
    fmt::print("Part 2 Solution: {}\n", part2(input));
}

#else

#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)";

    auto input = read_input(ss);

    REQUIRE(10 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(start-A
start-b
A-c
A-b
b-d
A-end
b-end)";

    auto input = read_input(ss);

    REQUIRE(36 == part2(input));
}

#endif
