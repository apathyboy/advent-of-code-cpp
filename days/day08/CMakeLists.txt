add_executable(day08 main.cpp)
target_link_libraries(
    day08
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)
