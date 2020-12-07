add_executable(day07 main.cpp)
target_link_libraries(
    day07
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)
