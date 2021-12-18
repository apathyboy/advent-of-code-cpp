#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <fstream>
#include <numeric>
#include <optional>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

struct SnailNumber {
    SnailNumber()              = default;
    SnailNumber(SnailNumber&&) = default;
    SnailNumber& operator=(SnailNumber&&) = default;

    SnailNumber(uint64_t value)
        : left_(nullptr)
        , right_(nullptr)
        , value_(value)
    {
    }
    SnailNumber(const SnailNumber&);
    SnailNumber& operator=(const SnailNumber&);

    explicit operator uint64_t() const;

    bool splits();
    bool explodes();

    bool lr_search(uint32_t depth, std::optional<uint64_t*>& last_value, std::optional<uint64_t>& spill);

private:
    std::unique_ptr<SnailNumber> left_;
    std::unique_ptr<SnailNumber> right_;
    uint64_t                     value_;

    bool is_terminal() const { return left_ == nullptr && right_ == nullptr; }

    friend std::istream& operator>>(std::istream&, SnailNumber&);
    friend std::ostream& operator<<(std::ostream&, const SnailNumber&);
    friend SnailNumber   operator+(const SnailNumber& left, const SnailNumber& right);
};

SnailNumber::SnailNumber(const SnailNumber& other)
    : left_(nullptr)
    , right_(nullptr)
    , value_(other.value_)
{
    if (other.is_terminal()) return;
    left_  = std::make_unique<SnailNumber>(*other.left_);
    right_ = std::make_unique<SnailNumber>(*other.right_);
}

SnailNumber& SnailNumber::operator=(const SnailNumber& other)
{
    if (this == &other) return *this;
    if (other.is_terminal()) {
        value_ = other.value_;
        left_  = nullptr;
        right_ = nullptr;
        return *this;
    }

    left_  = std::make_unique<SnailNumber>(*other.left_);
    right_ = std::make_unique<SnailNumber>(*other.right_);
    return *this;
}

std::istream& operator>>(std::istream& s, SnailNumber& number)
{
    char c;
    if (!(s >> c)) return s;
    if (c != '[') throw std::runtime_error("Expected left bracket.");
    number.left_ = std::make_unique<SnailNumber>();
    if (s.peek() == '[') { s >> *number.left_; }
    else {
        s >> number.left_->value_;
    }
    if (!(s >> c) || c != ',')
        throw std::runtime_error("Expected delimiter between left and right side of number.");
    number.right_ = std::make_unique<SnailNumber>();
    if (s.peek() == '[') { s >> *number.right_; }
    else {
        s >> number.right_->value_;
    }
    if (!(s >> c) || c != ']') throw std::runtime_error("Expeted right bracket.");
    return s;
}

std::ostream& operator<<(std::ostream& s, const SnailNumber& number)
{
    if (number.is_terminal()) {
        s << number.value_;
        return s;
    }
    s << "[" << *number.left_ << "," << *number.right_ << "]";
    return s;
}

SnailNumber::operator uint64_t() const
{
    if (is_terminal()) return value_;
    return 3 * static_cast<uint64_t>(*left_) + 2 * static_cast<uint64_t>(*right_);
}

SnailNumber operator+(const SnailNumber& left, const SnailNumber& right)
{
    SnailNumber result;
    result.left_  = std::make_unique<SnailNumber>(left);
    result.right_ = std::make_unique<SnailNumber>(right);
    do {
        while (result.explodes())
            ;
    } while (result.splits());
    return result;
}

bool SnailNumber::splits()
{
    if (!is_terminal()) {
        if (left_->splits()) return true;
        return right_->splits();
    }

    if (value_ < 10) return false;

    left_  = std::make_unique<SnailNumber>(value_ / 2);
    right_ = std::make_unique<SnailNumber>((value_ + 1) / 2);
    return true;
}

bool SnailNumber::explodes()
{
    std::optional<uint64_t>  spill;
    std::optional<uint64_t*> last_value;
    return lr_search(0, last_value, spill) || spill.has_value();
}

bool SnailNumber::lr_search(
    uint32_t                  depth,
    std::optional<uint64_t*>& last_value,
    std::optional<uint64_t>&  spill)
{
    if (is_terminal()) {
        if (spill.has_value()) {
            value_ += spill.value();
            spill.reset();
            return true;
        }

        last_value = &value_;
        return false;
    }

    if (depth < 4 || spill.has_value() || !left_->is_terminal() || !right_->is_terminal()) {
        return left_->lr_search(depth + 1, last_value, spill)
               || right_->lr_search(depth + 1, last_value, spill);
    }

    // depth >= 4 && !spill.has_value() && left->is_terminal() && right->is_terminal()
    if (last_value.has_value()) { (*last_value.value()) += left_->value_; }
    spill  = right_->value_;
    left_  = nullptr;
    right_ = nullptr;
    value_ = 0;
    return false;
}

uint64_t part1(std::vector<SnailNumber> numbers)
{
    SnailNumber sum = std::accumulate(numbers.begin() + 1, numbers.end(), numbers[0], std::plus<>());
    return static_cast<uint64_t>(sum);
    // std::cout << "The magnitude of the final sum is " << static_cast<uint64_t>(sum) << "\n";

    uint64_t best = 0;
    for (size_t i = 0; i < numbers.size(); i++) {
        for (size_t j = i + 1; j < numbers.size(); j++) {
            best = std::max(
                {best,
                 static_cast<uint64_t>(numbers[i] + numbers[j]),
                 static_cast<uint64_t>(numbers[j] + numbers[i])});
        }
    }
    // std::cout << "Maximum reachable magnitude is " << best << "\n";
    return 0;
}

uint64_t part2(std::vector<SnailNumber> numbers)
{
    uint64_t best = 0;
    for (size_t i = 0; i < numbers.size(); i++) {
        for (size_t j = i + 1; j < numbers.size(); j++) {
            best = std::max(
                {best,
                 static_cast<uint64_t>(numbers[i] + numbers[j]),
                 static_cast<uint64_t>(numbers[j] + numbers[i])});
        }
    }
    // std::cout << "Maximum reachable magnitude is " << best << "\n";
    return best;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 18\n");

    std::ifstream            ifs{"puzzle.in"};
    std::vector<SnailNumber> numbers;
    rs::copy(rs::istream_view<SnailNumber>(ifs), std::back_inserter(numbers));

    fmt::print("Part 1 Solution: {}\n", part1(numbers));
    fmt::print("Part 2 Solution: {}\n", part2(numbers));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"([[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]])";

    std::vector<SnailNumber> numbers;
    rs::copy(rs::istream_view<SnailNumber>(ss), std::back_inserter(numbers));

    REQUIRE(4140 == part1(numbers));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"([[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]
[[[5,[2,8]],4],[5,[[9,9],0]]]
[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]
[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]
[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]
[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]
[[[[5,4],[7,7]],8],[[8,3],8]]
[[9,3],[[9,9],[6,[4,9]]]]
[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]
[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]])";

    std::vector<SnailNumber> numbers;
    rs::copy(rs::istream_view<SnailNumber>(ss), std::back_inserter(numbers));

    REQUIRE(3993 == part2(numbers));
}

#endif