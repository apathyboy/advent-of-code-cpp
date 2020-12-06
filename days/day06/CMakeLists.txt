add_executable(day06 main.cpp)
target_link_libraries(
    day06
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(day06 PRIVATE -wd4459)
