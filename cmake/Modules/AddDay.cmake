include(CMakeParseArguments)

function(AddDay)
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
                -wd4201
                -wd4996
                -wd4459 # TODO range-v3 error
                -wd4702>) # TODO range-v3 error

    target_link_libraries(
        ${DAY_NAME}
        PRIVATE aoc
                fmt::fmt
                range-v3::meta
                ${DAY_LIBS})

    add_executable(${DAY_NAME}_tests ${DAY_NAME}/main.cpp)

    target_link_libraries(
        ${DAY_NAME}_tests
        PRIVATE aoc
                fmt::fmt
                range-v3::meta
                ${DAY_LIBS})

    target_compile_options(
        ${DAY_NAME}_tests
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4201
                -wd4996
                -wd4459 # TODO range-v3 error
                -wd4702 # TODO range-v3 error
                -wd6330>) # TODO catch2 error

    target_compile_definitions(${DAY_NAME}_tests PRIVATE UNIT_TESTING)

    catch_discover_tests(${DAY_NAME}_tests)

endfunction()