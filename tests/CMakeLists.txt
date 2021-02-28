cmake_minimum_required(VERSION 3.0.0)

add_executable(
    tests
    example_test.cpp
    # bill_test.cpp
    # util_test.cpp
    # details_test.cpp
    )

target_link_libraries(tests gtest_main app)

add_test(
    NAME
        tests
    COMMAND
        ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}/tests
    )

set_property(TARGET tests PROPERTY CXX_STANDARD 17)
