add_executable(day05 main.cpp)
target_link_libraries(
    day05
    PRIVATE aoc2020
            fmt::fmt
            range-v3::meta)

target_compile_options(day05 PRIVATE -wd4459)
