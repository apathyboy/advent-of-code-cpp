#include <aoc/aoc.hpp>

#include <fmt/core.h>
#include <glm/vec3.hpp>
#include <range/v3/all.hpp>

#include <deque>
#include <fstream>
#include <optional>
#include <sstream>
#include <unordered_set>

namespace rs = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;


using Rotation                 = std::array<std::pair<int, int>, 3>;
inline const constexpr int X   = 0;
inline const constexpr int Y   = 1;
inline const constexpr int Z   = 2;
inline const constexpr int POS = 1;
inline const constexpr int NEG = -1;

inline const constexpr std::array<Rotation, 24> ROTATIONS({
    {{{X, POS}, {Y, POS}, {Z, POS}}}, {{{X, POS}, {Z, POS}, {Y, NEG}}}, {{{X, POS}, {Y, NEG}, {Z, NEG}}},
    {{{X, POS}, {Z, NEG}, {Y, POS}}}, {{{X, NEG}, {Y, POS}, {Z, NEG}}}, {{{X, NEG}, {Z, NEG}, {Y, NEG}}},
    {{{X, NEG}, {Y, NEG}, {Z, POS}}}, {{{X, NEG}, {Z, POS}, {Y, POS}}}, {{{Y, POS}, {X, POS}, {Z, NEG}}},
    {{{Y, POS}, {Z, NEG}, {X, NEG}}}, {{{Y, POS}, {X, NEG}, {Z, POS}}}, {{{Y, POS}, {Z, POS}, {X, POS}}},
    {{{Y, NEG}, {X, POS}, {Z, POS}}}, {{{Y, NEG}, {Z, POS}, {X, NEG}}}, {{{Y, NEG}, {X, NEG}, {Z, NEG}}},
    {{{Y, NEG}, {Z, NEG}, {X, POS}}}, {{{Z, POS}, {X, POS}, {Y, POS}}}, {{{Z, POS}, {Y, POS}, {X, NEG}}},
    {{{Z, POS}, {X, NEG}, {Y, NEG}}}, {{{Z, POS}, {Y, NEG}, {X, POS}}}, {{{Z, NEG}, {X, POS}, {Y, NEG}}},
    {{{Z, NEG}, {Y, NEG}, {X, NEG}}}, {{{Z, NEG}, {X, NEG}, {Y, POS}}}, {{{Z, NEG}, {Y, POS}, {X, POS}}},
});

struct Point3D {
    int64_t              x;
    int64_t              y;
    int64_t              z;
    Point3D              rotate(const Rotation&) const;
    int64_t              manhattan() { return std::abs(x) + std::abs(y) + std::abs(z); }
    friend std::istream& operator>>(std::istream&, Point3D&);
    friend std::ostream& operator<<(std::ostream&, const Point3D&);
    friend Point3D       operator-(const Point3D&, const Point3D&);
    friend Point3D       operator+(const Point3D&, const Point3D&);
    friend auto          operator<=>(const Point3D&, const Point3D&) = default;
};

template <>
struct std::hash<Point3D> {
    std::size_t operator()(Point3D const& s) const noexcept
    {
        std::size_t h1 = std::hash<int64_t>{}(s.x);
        std::size_t h2 = std::hash<int64_t>{}(s.y);
        std::size_t h3 = std::hash<int64_t>{}(s.z);
        return h1 ^ (h2 << 1) ^ (h3 << 2); // or use boost::hash_combine
    }
};

std::istream& operator>>(std::istream& s, Point3D& b)
{
    char delim;
    int  coord;

    if (!(s >> coord)) { return s; }
    b.x = coord;

    if (!(s >> delim) || delim != ',') throw std::runtime_error("Found unexpected delimiter.");

    if (!(s >> b.y >> delim >> b.z) || delim != ',')
        throw std::runtime_error("Failed to parse y and z coordinates.");
    return s;
}

std::ostream& operator<<(std::ostream& s, const Point3D& b)
{
    s << b.x << "," << b.y << "," << b.z;
    return s;
}

Point3D Point3D::rotate(const Rotation& rot) const
{
    const int64_t* vals[3] = {&x, &y, &z};
    return {
        (*vals[rot[0].first]) * rot[0].second,
        (*vals[rot[1].first]) * rot[1].second,
        (*vals[rot[2].first]) * rot[2].second};
}

Point3D operator-(const Point3D& lhs, const Point3D& rhs)
{
    return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}
Point3D operator+(const Point3D& lhs, const Point3D& rhs)
{
    return {lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
}

using Beacon = Point3D;

struct ScannerOrientation {
    Rotation rotation;
    Point3D  position;
};

struct Scanner {
    int64_t             scanner_id;
    std::vector<Beacon> beacons;
    Point3D             position;

    std::optional<ScannerOrientation> overlaps(const Scanner& other, uint64_t threshold) const;
    void                              normalize(const ScannerOrientation&);

    friend std::istream& operator>>(std::istream&, Scanner&);
    friend std::ostream& operator<<(std::ostream&, const Scanner&);
};

void normalize_scanners(std::vector<Scanner>& scanners, uint64_t threshold);

std::istream& operator>>(std::istream& s, Scanner& scan)
{
    std::string       line;
    std::string_view  header = "--- scanner ";
    std::stringstream parse;
    scan.beacons.clear();
    while (getline(s, line)) {
        if (line == "") break;
        if (line.starts_with(header)) {
            scan.scanner_id = std::stoll(line.substr(header.length()));
            continue;
        }
        parse.clear();
        parse.str(line);
        Beacon b;
        if (parse >> b) scan.beacons.push_back(b);
        else
            return s;
    }
    return s;
}

std::ostream& operator<<(std::ostream& s, const Scanner& scan)
{
    s << "--- scanner " << scan.scanner_id << " ---\n";
    std::ranges::copy(scan.beacons, std::ostream_iterator<Beacon>(s, "\n"));
    return s;
}

std::optional<ScannerOrientation> Scanner::overlaps(const Scanner& other, uint64_t threshold) const
{
    for (auto& rotation : ROTATIONS) {
        std::unordered_map<Beacon, uint32_t> cnts;
        for (auto& lhs : beacons)
            for (auto& rhs : other.beacons)
                cnts[lhs - rhs.rotate(rotation)]++;

        for (auto& el : cnts)
            if (el.second >= threshold) return ScannerOrientation{rotation, el.first};
    }
    return {};
}

void Scanner::normalize(const ScannerOrientation& orientation)
{
    for (auto& beacon : beacons)
        beacon = beacon.rotate(orientation.rotation) + orientation.position;
    position = orientation.position;
}

void normalize_scanners(std::vector<Scanner>& scanners, uint64_t threshold)
{
    std::unordered_set<int64_t> fixed;
    std::deque<int64_t>         queue;
    fixed.insert(0);
    queue.push_back(0);
    scanners[0].position = Point3D{0, 0, 0};
    while (!queue.empty()) {
        auto tested = queue.front();
        queue.pop_front();
        for (size_t other = 0; other < scanners.size(); other++) {
            if (fixed.contains(other)) continue;
            auto rot = scanners[tested].overlaps(scanners[other], threshold);
            if (!rot.has_value()) continue;
            scanners[other].normalize(rot.value());
            queue.push_back(other);
            fixed.insert(other);
        }
    }
}

std::vector<Scanner> read_input(std::istream& input)
{
    std::vector<Scanner> scanners;

    do {
        Scanner tmp;

        input >> tmp;

        scanners.push_back(tmp);
    } while (!input.eof());

    // rs::copy(rs::istream_view<Scanner>(input), std::back_insert_iterator(scanners));
    //  if (scanners.size() != 26) throw std::runtime_error("Failed to read input.");
    normalize_scanners(scanners, 12);

    return scanners;
}

int64_t part1(const std::vector<Scanner>& scanners)
{
    std::unordered_set<Beacon> unique_beacons;
    for (const auto& s : scanners)
        for (const auto& b : s.beacons)
            unique_beacons.insert(b);
    return unique_beacons.size();
}

int64_t part2(const std::vector<Scanner>& scanners)
{
    int64_t best = 0;
    for (auto lhs : scanners)
        for (auto rhs : scanners)
            best = std::max(best, (lhs.position - rhs.position).manhattan());

    return best;
}

#ifndef UNIT_TESTING

int main()
{
    fmt::print("Advent of Code 2021 - Day 19\n");

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
    ss << R"(--- scanner 0 ---
404,-588,-901
528,-643,409
-838,591,734
390,-675,-793
-537,-823,-458
-485,-357,347
-345,-311,381
-661,-816,-575
-876,649,763
-618,-824,-621
553,345,-567
474,580,667
-447,-329,318
-584,868,-557
544,-627,-890
564,392,-477
455,729,728
-892,524,684
-689,845,-530
423,-701,434
7,-33,-71
630,319,-379
443,580,662
-789,900,-551
459,-707,401

--- scanner 1 ---
686,422,578
605,423,415
515,917,-361
-336,658,858
95,138,22
-476,619,847
-340,-569,-846
567,-361,727
-460,603,-452
669,-402,600
729,430,532
-500,-761,534
-322,571,750
-466,-666,-811
-429,-592,574
-355,545,-477
703,-491,-529
-328,-685,520
413,935,-424
-391,539,-444
586,-435,557
-364,-763,-893
807,-499,-711
755,-354,-619
553,889,-390

--- scanner 2 ---
649,640,665
682,-795,504
-784,533,-524
-644,584,-595
-588,-843,648
-30,6,44
-674,560,763
500,723,-460
609,671,-379
-555,-800,653
-675,-892,-343
697,-426,-610
578,704,681
493,664,-388
-671,-858,530
-667,343,800
571,-461,-707
-138,-166,112
-889,563,-600
646,-828,498
640,759,510
-630,509,768
-681,-892,-333
673,-379,-804
-742,-814,-386
577,-820,562

--- scanner 3 ---
-589,542,597
605,-692,669
-500,565,-823
-660,373,557
-458,-679,-417
-488,449,543
-626,468,-788
338,-750,-386
528,-832,-391
562,-778,733
-938,-730,414
543,643,-506
-524,371,-870
407,773,750
-104,29,83
378,-903,-323
-778,-728,485
426,699,580
-438,-605,-362
-469,-447,-387
509,732,623
647,635,-688
-868,-804,481
614,-800,639
595,780,-596

--- scanner 4 ---
727,592,562
-293,-554,779
441,611,-461
-714,465,-776
-743,427,-804
-660,-479,-426
832,-632,460
927,-485,-438
408,393,-506
466,436,-512
110,16,151
-258,-428,682
-393,719,612
-211,-452,876
808,-476,-593
-575,615,604
-485,667,467
-680,325,-822
-627,-443,-432
872,-547,-609
833,512,582
807,604,487
839,-516,451
891,-625,532
-652,-548,-490
30,-46,-14)";

    auto input = read_input(ss);

    REQUIRE(79 == part1(input));
}

TEST_CASE("Can solve part 2 example")
{
    std::stringstream ss;
    ss << R"(--- scanner 0 ---
404,-588,-901
528,-643,409
-838,591,734
390,-675,-793
-537,-823,-458
-485,-357,347
-345,-311,381
-661,-816,-575
-876,649,763
-618,-824,-621
553,345,-567
474,580,667
-447,-329,318
-584,868,-557
544,-627,-890
564,392,-477
455,729,728
-892,524,684
-689,845,-530
423,-701,434
7,-33,-71
630,319,-379
443,580,662
-789,900,-551
459,-707,401

--- scanner 1 ---
686,422,578
605,423,415
515,917,-361
-336,658,858
95,138,22
-476,619,847
-340,-569,-846
567,-361,727
-460,603,-452
669,-402,600
729,430,532
-500,-761,534
-322,571,750
-466,-666,-811
-429,-592,574
-355,545,-477
703,-491,-529
-328,-685,520
413,935,-424
-391,539,-444
586,-435,557
-364,-763,-893
807,-499,-711
755,-354,-619
553,889,-390

--- scanner 2 ---
649,640,665
682,-795,504
-784,533,-524
-644,584,-595
-588,-843,648
-30,6,44
-674,560,763
500,723,-460
609,671,-379
-555,-800,653
-675,-892,-343
697,-426,-610
578,704,681
493,664,-388
-671,-858,530
-667,343,800
571,-461,-707
-138,-166,112
-889,563,-600
646,-828,498
640,759,510
-630,509,768
-681,-892,-333
673,-379,-804
-742,-814,-386
577,-820,562

--- scanner 3 ---
-589,542,597
605,-692,669
-500,565,-823
-660,373,557
-458,-679,-417
-488,449,543
-626,468,-788
338,-750,-386
528,-832,-391
562,-778,733
-938,-730,414
543,643,-506
-524,371,-870
407,773,750
-104,29,83
378,-903,-323
-778,-728,485
426,699,580
-438,-605,-362
-469,-447,-387
509,732,623
647,635,-688
-868,-804,481
614,-800,639
595,780,-596

--- scanner 4 ---
727,592,562
-293,-554,779
441,611,-461
-714,465,-776
-743,427,-804
-660,-479,-426
832,-632,460
927,-485,-438
408,393,-506
466,436,-512
110,16,151
-258,-428,682
-393,719,612
-211,-452,876
808,-476,-593
-575,615,604
-485,667,467
-680,325,-822
-627,-443,-432
872,-547,-609
833,512,582
807,604,487
839,-516,451
891,-625,532
-652,-548,-490
30,-46,-14)";

    auto input = read_input(ss);

    REQUIRE(3621 == part2(input));
}

#endif
