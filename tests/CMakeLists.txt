cmake_minimum_required(VERSION 3.0.0)
set_property(TARGET app PROPERTY CXX_STANDARD 17)

file(GLOB src CONFIGURE_DEPENDS
    "../src/utils.cpp"
    "../src/bill.cpp"
    "../src/details.cpp"
    "../src/invoice.cpp"
    "../src/fort.cpp"
    "../src/include/*.h"
    "../src/include/*.hpp"
    "*.h"
    ".hpp"
    "*.cpp"
    "*.c"
)

add_executable(tests ${src})
target_link_libraries(tests PUBLIC gtest)
add_test(NAME tests COMMAND tests)

set_property(TARGET tests PROPERTY CXX_STANDARD 17)
