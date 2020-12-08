add_executable(day09 main.cpp)
target_link_libraries(
    day09
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)
