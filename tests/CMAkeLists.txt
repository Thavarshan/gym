cmake_minimum_required(VERSION 3.0.0)

file(GLOB src CONFIGURE_DEPENDS
    "*.h"
    ".hpp"
    "*.cpp"
    "*.c"
)

add_executable(tests ${src})
target_link_libraries(tests PUBLIC gtest)
add_test(NAME tests COMMAND tests)
