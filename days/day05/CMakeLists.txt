add_executable(day05 main.cpp)
target_link_libraries(
    day05
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(day05 PRIVATE -wd4459)

add_executable(day05_tests main.cpp)

target_link_libraries(
    day05_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day05_tests
    PRIVATE -wd4459
            -wd4702
            -wd6330)

target_compile_definitions(day05_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day05_tests)
