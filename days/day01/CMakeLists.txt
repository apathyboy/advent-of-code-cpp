add_executable(day01 main.cpp)
target_link_libraries(
    day01
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day01_tests main.cpp)

target_link_libraries(
    day01_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day01_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day01_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day01_tests)
