cmake_minimum_required(VERSION 3.0.0)
project(gym VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB src CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable(gym ${src})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
