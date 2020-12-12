add_executable(day02 main.cpp)

target_link_libraries(
    day02
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

add_executable(day02_tests main.cpp)

target_link_libraries(
    day02_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day02_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day02_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day02_tests)
