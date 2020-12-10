add_executable(day11 main.cpp)

target_compile_options(
    day11
    PRIVATE -wd4459
            -wd4702)

target_link_libraries(
    day11
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day11_tests main.cpp)

target_link_libraries(
    day11_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day11_tests
    PRIVATE -wd4459
            -wd4702
            -wd6330)

target_compile_definitions(day11_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day11_tests)
