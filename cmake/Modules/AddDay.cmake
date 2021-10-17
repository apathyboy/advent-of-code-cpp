include(CMakeParseArguments)

function(AddDay)
    cmake_parse_arguments(
        PUZZLE # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "YEAR;DAY" # list of names of mono-valued arguments
        "LIBS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

    set(APP_NAME ${PUZZLE_YEAR}_${PUZZLE_DAY})

    add_executable(${APP_NAME} ${PUZZLE_DAY}/main.cpp)

    target_compile_options(
        ${APP_NAME}
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4201
                -wd4996
                -wd4459 # TODO range-v3 error
                -wd4702 # TODO range-v3 error
                -wd6330>)

    target_link_libraries(
        ${APP_NAME}
        PRIVATE aoc
                fmt::fmt
                range-v3::meta
                ${PUZZLE_LIBS})

    add_executable(${APP_NAME}_tests ${PUZZLE_DAY}/main.cpp)

    target_link_libraries(
        ${APP_NAME}_tests
        PRIVATE aoc
                fmt::fmt
                range-v3::meta
                ${PUZZLE_LIBS})

    target_compile_options(
        ${APP_NAME}_tests
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4201
                -wd4996
                -wd4459 # TODO range-v3 error
                -wd4702 # TODO range-v3 error
                -wd6330>) # TODO catch2 error

    target_compile_definitions(${APP_NAME}_tests PRIVATE UNIT_TESTING)

    catch_discover_tests(${APP_NAME}_tests)
endfunction()