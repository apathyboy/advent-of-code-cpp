find_package(
    Catch2
    CONFIG
    REQUIRED)

include(CTest)
include(Catch)

add_executable(
    aoc2020-tests
    day01.cpp
    day02.cpp
    day05.cpp
    main.cpp)

target_link_libraries(
    aoc2020-tests
    PRIVATE Catch2::Catch2
            esb::aoc2020
            range-v3::meta)

target_compile_options(aoc2020-tests PRIVATE -wd6330)

catch_discover_tests(aoc2020-tests)
