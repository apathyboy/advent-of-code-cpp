include(CMakeParseArguments)

function(add_day)
    cmake_parse_arguments(
        DAY # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "NAME" # list of names of mono-valued arguments
        "LIBS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

    add_executable(${DAY_NAME} ${DAY_NAME}/main.cpp)

    target_compile_options(
        ${DAY_NAME}
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4996
                -wd4459 # TODO range-v3 error
                -wd4702>) # TODO range-v3 error

    target_link_libraries(
        ${DAY_NAME}
        PRIVATE aoc2020
                fmt::fmt
                range-v3::meta
                ${DAY_LIBS})

    add_executable(${DAY_NAME}_tests ${DAY_NAME}/main.cpp)

    target_link_libraries(
        ${DAY_NAME}_tests
        PRIVATE aoc2020
                fmt::fmt
                range-v3::meta
                ${DAY_LIBS})

    target_compile_options(
        ${DAY_NAME}_tests
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4996
                -wd4459 # TODO range-v3 error
                -wd4702 # TODO range-v3 error
                -wd6330>) # TODO catch2 error

    target_compile_definitions(${DAY_NAME}_tests PRIVATE UNIT_TESTING)

    catch_discover_tests(${DAY_NAME}_tests)

endfunction()

# cmake-format: off
add_day(NAME day01)
add_day(NAME day02)
add_day(NAME day03)
add_day(NAME day04)
add_day(NAME day05)
add_day(NAME day06)
add_day(NAME day07)
add_day(NAME day08)
add_day(NAME day09)
add_day(NAME day10)
add_day(NAME day11)
add_day(NAME day12 LIBS glm)
add_day(NAME day13)
add_day(NAME day14)
add_day(NAME day15)
add_day(NAME day16)
add_day(NAME day17)
add_day(NAME day18)
add_day(NAME day19)
add_day(NAME day20)
add_day(NAME day21)
add_day(NAME day22)
add_day(NAME day23)
add_day(NAME day24)
add_day(NAME day25)
# cmake-format: on
