add_executable(day05 main.cpp)

target_link_libraries(
    day05
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day05
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459>) # TODO range-v3 error

add_executable(day05_tests main.cpp)

target_link_libraries(
    day05_tests
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(
    day05_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day05_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day05_tests)
