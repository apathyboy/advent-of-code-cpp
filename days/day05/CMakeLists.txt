add_executable(day05 main.cpp)
target_link_libraries(
    day05
    PRIVATE aoc2020
            fmt::fmt)
target_compile_options(day05 PRIVATE $<$<CXX_COMPILER_ID:MSVC>:-wd28278>)
