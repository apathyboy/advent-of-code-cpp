add_executable(day03 main.cpp)

target_link_libraries(
    day03
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day03_tests main.cpp)

target_link_libraries(
    day03_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day03_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd6330>) # TODO catch2 error

target_compile_definitions(day03_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day03_tests)
