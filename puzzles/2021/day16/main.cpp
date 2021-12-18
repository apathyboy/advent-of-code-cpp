#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <range/v3/all.hpp>

#include <coroutine>
#include <fstream>
#include <numeric>
#include <optional>
#include <variant>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;


template <std::movable T>
class Generator {
public:
    struct promise_type {
        Generator<T>               get_return_object() { return Generator{Handle::from_promise(*this)}; }
        static std::suspend_always initial_suspend() noexcept { return {}; }
        static std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always        yield_value(T value) noexcept
        {
            current_value = std::move(value);
            return {};
        }
        // Disallow co_await in generator coroutines.
        void                     await_transform() = delete;
        [[noreturn]] static void unhandled_exception() { throw; }

        std::optional<T> current_value;
        void             return_void() noexcept {}
    };

    using Handle = std::coroutine_handle<promise_type>;

    explicit Generator(const Handle coroutine)
        : m_coroutine{coroutine}
    {
    }

    Generator() = default;
    ~Generator()
    {
        if (m_coroutine) { m_coroutine.destroy(); }
    }

    Generator(const Generator&) = delete;
    Generator& operator=(const Generator&) = delete;

    Generator(Generator&& other) noexcept
        : m_coroutine{other.m_coroutine}
    {
        other.m_coroutine = {};
    }
    Generator& operator=(Generator&& other) noexcept
    {
        if (this != &other) {
            if (m_coroutine) { m_coroutine.destroy(); }
            m_coroutine       = other.m_coroutine;
            other.m_coroutine = {};
        }
        return *this;
    }

    // Range-based for loop support.
    class Iter {
    public:
        void     operator++() { m_coroutine.resume(); }
        const T& operator*() const { return *m_coroutine.promise().current_value; }
        bool     operator==(std::default_sentinel_t) const { return !m_coroutine || m_coroutine.done(); }

        explicit Iter(const Handle coroutine)
            : m_coroutine{coroutine}
        {
        }

    private:
        Handle m_coroutine;
    };

    Iter begin()
    {
        if (m_coroutine) { m_coroutine.resume(); }
        return Iter{m_coroutine};
    }
    std::default_sentinel_t end() const { return {}; }

private:
    Handle m_coroutine;
};


uint32_t        hex_to_dec(char c);
Generator<bool> istream_reader(std::istream& s);

struct field_width_mod {
    uint8_t width;
};
inline const constexpr auto field_width = [](uint8_t width) { return field_width_mod{width}; };

struct BaseBitStream {
    BaseBitStream()
        : next_field_{}
        , done_(false)
    {
    }
    explicit                              operator bool() const { return !done_; }
    virtual std::pair<uint32_t, uint32_t> read_bits(uint32_t bit_cnt) = 0;

    friend BaseBitStream& operator>>(BaseBitStream& s, field_width_mod mod)
    {
        s.next_field_ = mod.width;
        return s;
    }

    friend BaseBitStream& operator>>(BaseBitStream& s, std::integral auto& value)
    {
        auto width = sizeof(value) * CHAR_BIT;
        if (s.next_field_.has_value()) { width = s.next_field_.value(); }
        auto [data, cnt] = s.read_bits(static_cast<uint32_t>(width));
        if (cnt != width) return s;
        value         = data;
        s.next_field_ = std::optional<uint8_t>();
        return s;
    }

    void set_done() { done_ = true; }

private:
    std::optional<uint8_t> next_field_;
    bool                   done_;
};

struct BitStream : BaseBitStream {
    BitStream(std::istream&);

    std::pair<uint32_t, uint32_t> read_bits(uint32_t bit_cnt) override;

private:
    Generator<bool>       gen_;
    Generator<bool>::Iter it_;
};

struct SubBitStream : BaseBitStream {
    SubBitStream(BaseBitStream& parent, uint32_t bit_limit);

    std::pair<uint32_t, uint32_t> read_bits(uint32_t bit_cnt) override;

    uint32_t limit() { return limit_; }

private:
    BaseBitStream* parent_;
    uint32_t       limit_;
};


inline uint32_t hex_to_dec(char c)
{
    if (isdigit(c)) return c - '0';
    if (isxdigit(c)) return c - 'A' + 10;
    throw std::runtime_error("Unexpected character conversion.");
}

Generator<bool> istream_reader(std::istream& s)
{
    for (uint32_t v : rs::istream_view<char>(s) | rv::transform(hex_to_dec)) {
        co_yield v & 0b1000;
        co_yield v & 0b100;
        co_yield v & 0b10;
        co_yield v & 0b1;
    }
}

BitStream::BitStream(std::istream& input)
    : gen_(istream_reader(input))
    , it_(gen_.begin())
{
}

std::pair<uint32_t, uint32_t> BitStream::read_bits(uint32_t bit_cnt)
{
    if (it_ == gen_.end()) {
        set_done();
        return {0, 0};
    }
    uint32_t result = 0;
    uint32_t read   = 0;
    for (auto i = it_; read < bit_cnt && i != gen_.end(); ++i, read++)
        result = (result << 1) | *i;
    return {result, read};
}

SubBitStream::SubBitStream(BaseBitStream& parent, uint32_t bit_limit)
    : parent_(std::addressof(parent))
    , limit_(bit_limit)
{
}

std::pair<uint32_t, uint32_t> SubBitStream::read_bits(uint32_t bit_cnt)
{
    if (limit_ == 0) {
        set_done();
        return {0, 0};
    }
    if (bit_cnt > limit_) throw std::runtime_error("Attempted to read past limit.");
    auto [data, read] = parent_->read_bits(bit_cnt);
    limit_ -= read;
    return {data, read};
}

namespace Packet {

struct Literal {
    uint64_t              data;
    friend BaseBitStream& operator>>(BaseBitStream&, Literal&);
};

struct LengthOrCount {
    bool     is_count;
    uint32_t size;

    friend BaseBitStream& operator>>(BaseBitStream&, LengthOrCount&);
};

struct Packet {
    uint32_t version;
    uint32_t type_id;
    struct SubPackets {
        LengthOrCount       size;
        std::vector<Packet> packets;
    };
    std::variant<Literal, SubPackets> data;

    bool                       is_literal() const;
    bool                       has_subpackets() const;
    uint64_t                   literal_value() const;
    const std::vector<Packet>& sub_packets() const;

    friend BaseBitStream& operator>>(BaseBitStream&, Packet&);

    operator uint64_t() const;
};

uint32_t sum_versions(const Packet& p);

} // namespace Packet


namespace Packet {

BaseBitStream& operator>>(BaseBitStream& s, Literal& lit)
{
    uint64_t value = 0;
    while (true) {
        uint32_t data = 0;
        if (!(s >> field_width(5) >> data)) throw std::runtime_error("Failed to read literal chunk.");
        value = (value << 4) | (data & UINT32_C(0b1111));
        if (!(data & UINT32_C(0b10000))) break;
    }
    lit.data = value;
    return s;
}

BaseBitStream& operator>>(BaseBitStream& s, LengthOrCount& cnt)
{
    if (!(s >> field_width(1) >> cnt.is_count)) return s;

    if (cnt.is_count) { s >> field_width(11) >> cnt.size; }
    else {
        s >> field_width(15) >> cnt.size;
    }
    return s;
}

BaseBitStream& operator>>(BaseBitStream& s, Packet& p)
{
    if (!(s >> field_width(3) >> p.version >> field_width(3) >> p.type_id)) { return s; }

    if (p.is_literal()) {
        Literal lit;
        s >> lit;
        p.data = lit;
    }
    else {
        Packet::SubPackets packets;
        s >> packets.size;
        if (packets.size.is_count) {
            for (uint32_t i = 0; i < packets.size.size; i++) {
                Packet subp;
                s >> subp;
                packets.packets.push_back(subp);
            }
        }
        else {
            SubBitStream stream(s, packets.size.size);
            Packet       subp;
            while (stream >> subp)
                packets.packets.push_back(subp);
        }
        p.data = packets;
    }
    return s;
}

uint32_t sum_versions(const Packet& p)
{
    if (std::holds_alternative<Literal>(p.data)) { return p.version; }
    else {
        uint32_t sum = p.version;
        for (auto& subp : p.sub_packets()) {
            sum += sum_versions(subp);
        }
        return sum;
    }
}

Packet::operator uint64_t() const
{
    switch (type_id) {
        case 0:
            return std::accumulate(
                sub_packets().begin(),
                sub_packets().end(),
                UINT64_C(0),
                std::plus<>());
        case 1:
            return std::accumulate(
                sub_packets().begin(),
                sub_packets().end(),
                UINT64_C(1),
                std::multiplies<>());
        case 2:
            return *std::ranges::min_element(sub_packets());
        case 3:
            return *std::ranges::max_element(sub_packets());
        case 4:
            return literal_value();
        case 5:
            return static_cast<uint64_t>(sub_packets()[0]) > static_cast<uint64_t>(sub_packets()[1]);
        case 6:
            return static_cast<uint64_t>(sub_packets()[0]) < static_cast<uint64_t>(sub_packets()[1]);
        case 7:
            return static_cast<uint64_t>(sub_packets()[0]) == static_cast<uint64_t>(sub_packets()[1]);
    }
    return 0;
}

bool Packet::is_literal() const
{
    return type_id == 4;
}
bool Packet::has_subpackets() const
{
    return type_id != 4 && std::holds_alternative<SubPackets>(data);
}
uint64_t Packet::literal_value() const
{
    return std::get<Literal>(data).data;
}
const std::vector<Packet>& Packet::sub_packets() const
{
    return std::get<SubPackets>(data).packets;
}

} // namespace Packet

int part1(const Packet::Packet& packet)
{
    return sum_versions(packet);
}

uint64_t part2(const Packet::Packet& packet)
{
    return static_cast<uint64_t>(packet);
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 16\n");

    std::ifstream ifs{"puzzle.in"};

    BitStream      stream(ifs);
    Packet::Packet packet;
    stream >> packet;

    fmt::print("Part 1 Solution: {}\n", part1(packet));
    fmt::print("Part 2 Solution: {}\n", part2(packet));
}

#else

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>

TEST_CASE("Can solve part 1 example")
{
    std::stringstream ss;
    ss << R"(8A004A801A8002F478)";

    BitStream      stream(ss);
    Packet::Packet packet;
    stream >> packet;

    REQUIRE(16 == part1(packet));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(C200B40A82)";

    BitStream      stream(ss);
    Packet::Packet packet;
    stream >> packet;

    REQUIRE(3 == part2(packet));
}

#endif