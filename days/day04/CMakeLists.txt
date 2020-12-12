add_executable(day04 main.cpp)

target_link_libraries(
    day04
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day04_tests main.cpp)

target_link_libraries(
    day04_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day04_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day04_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day04_tests)
