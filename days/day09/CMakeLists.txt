add_executable(day09 main.cpp)

target_compile_options(day09 PRIVATE -wd4459)

target_link_libraries(
    day09
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day09_tests main.cpp)

target_link_libraries(
    day09_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day09_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day09_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day09_tests)
