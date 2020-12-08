add_executable(day08 main.cpp)

target_link_libraries(
    day08
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day08_tests main.cpp)

target_link_libraries(
    day08_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(day08_tests PRIVATE -wd6330)
target_compile_definitions(day08_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day08_tests)
