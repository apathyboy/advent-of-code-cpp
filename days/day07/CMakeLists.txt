add_executable(day07 main.cpp)

target_link_libraries(
    day07
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day07_tests main.cpp)

target_link_libraries(
    day07_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day07_tests
    PRIVATE -wd4459
            -wd6330)

target_compile_definitions(day07_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day07_tests)
