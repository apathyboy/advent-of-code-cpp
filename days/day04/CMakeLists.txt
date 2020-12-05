add_executable(day04 main.cpp)
target_link_libraries(
    day04
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)
target_include_directories(day04 PRIVATE ${BOOST_ALGORITHM_INCLUDE_DIRS})
