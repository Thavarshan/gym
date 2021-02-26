cmake_minimum_required(VERSION 3.0.0)
project(GYM_APP VERSION 1.0.0)

include(CTest)
enable_testing()

file(GLOB src CONFIGURE_DEPENDS
    "./src/*.h"
    "./src/.hpp"
    "./src/*.cpp"
    "./src/*.c"
)

add_subdirectory(googletest)
include_directories(googletest/googletest/include)

add_executable(app ${src})
add_subdirectory(tests)

set_property(TARGET app PROPERTY CXX_STANDARD 17)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
