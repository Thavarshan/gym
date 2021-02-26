cmake_minimum_required(VERSION 3.0.0)

file(GLOB src CONFIGURE_DEPENDS
    "../src/utils.cpp"
    "../src/bill.cpp"
    "../src/details.cpp"
    "../src/invoice.cpp"
    "../src/fort.cpp"
    "../src/includes/*.h"
    "../src/includes/*.hpp"
    "*.h"
    ".hpp"
    "*.cpp"
    "*.c"
)

add_executable(tests ${src})
target_link_libraries(tests PUBLIC gtest)
add_test(NAME tests COMMAND tests)

set_property(TARGET gym PROPERTY CXX_STANDARD 17)
