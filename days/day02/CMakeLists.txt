add_executable(day02 main.cpp)
target_link_libraries(
    day02
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)
