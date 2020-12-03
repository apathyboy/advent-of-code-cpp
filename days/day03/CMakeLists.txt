add_executable(day03 main.cpp)
target_link_libraries(
    day03
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)
