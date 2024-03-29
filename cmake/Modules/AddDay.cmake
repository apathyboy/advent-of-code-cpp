include(CMakeParseArguments)

function(AddDay)
    cmake_parse_arguments(
        PUZZLE # prefix of output variables
        "" # list of names of the boolean arguments (only defined ones will be true)
        "YEAR;DAY" # list of names of mono-valued arguments
        "LIBS" # list of names of multi-valued arguments (output variables are lists)
        ${ARGN} # arguments of the function to parse, here we take the all original ones
    )

    set(APP_NAME_PREFIX ${PUZZLE_YEAR}_${PUZZLE_DAY})

    add_executable(${APP_NAME_PREFIX}_app ${PUZZLE_DAY}/main.cpp)

    target_compile_options(
        ${APP_NAME_PREFIX}_app
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4201
                -wd4996
                -wd28278
                -wd4805
                -wd4459 # TODO range-v3 error
                -wd4702 # TODO range-v3 error
                -wd6330>)

    target_link_libraries(
        ${APP_NAME_PREFIX}_app
        PRIVATE aoc
                fmt::fmt
                glm::glm
                range-v3::meta
                ${PUZZLE_LIBS})

    add_executable(${APP_NAME_PREFIX}_tests ${PUZZLE_DAY}/main.cpp)

    target_link_libraries(
        ${APP_NAME_PREFIX}_tests
        PRIVATE aoc
                Catch2::Catch2WithMain
                fmt::fmt
                glm::glm
                range-v3::meta
                ${PUZZLE_LIBS})

    target_compile_options(
        ${APP_NAME_PREFIX}_tests
        PRIVATE $<$<CXX_COMPILER_ID:MSVC>:
                -wd4201
                -wd4996
                -wd28278
                -wd4805
                -wd4459 # TODO range-v3 error
                -wd4702 # TODO range-v3 error
                -wd6330>) # TODO catch2 error

    target_compile_definitions(${APP_NAME_PREFIX}_tests PRIVATE UNIT_TESTING)

    catch_discover_tests(${APP_NAME_PREFIX}_tests)

    add_custom_target(${APP_NAME_PREFIX} DEPENDS ${APP_NAME_PREFIX}_app
                                                 ${APP_NAME_PREFIX}_tests)
endfunction()
