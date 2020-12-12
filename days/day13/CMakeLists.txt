add_executable(day13 main.cpp)

target_compile_options(
    day13
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd4702>) # TODO range-v3 error

target_link_libraries(
    day13
    PRIVATE aoc2020
            fmt::fmt
            glm
            range-v3::meta)

add_executable(day13_tests main.cpp)

target_link_libraries(
    day13_tests
    PRIVATE aoc2020
            fmt::fmt
            glm
            range-v3::meta)

target_compile_options(
    day13_tests
    PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
            -wd4459 # TODO range-v3 error
            -wd4702 # TODO range-v3 error
            -wd6330>) # TODO catch2 error

target_compile_definitions(day13_tests PRIVATE UNIT_TESTING)

catch_discover_tests(day13_tests)
