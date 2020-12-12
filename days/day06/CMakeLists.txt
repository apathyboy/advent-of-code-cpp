add_executable(day06 main.cpp)

target_link_libraries(
    day06
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day06
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459>) # TODO range-v3 error

add_executable(day06_tests main.cpp)

target_link_libraries(
    day06_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day06_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day06_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day06_tests)
