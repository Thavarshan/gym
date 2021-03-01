cmake_minimum_required(VERSION 3.0.0)
project(GYM_APP VERSION 1.0.0)

include(CTest)
enable_testing()

include(GNUInstallDirs)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    )
foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

file(GLOB src CONFIGURE_DEPENDS
    "./src/includes/*.h"
    "./src/includes/.hpp"
    "./src/*.cpp"
    "./src/*.c"
)

add_executable(app ${src})

if(CMAKE_BUILD_TYPE MATCHES Debug)
    add_subdirectory(googletest)
    include_directories(googletest/googletest/include)
    add_subdirectory(tests)
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set_property(TARGET app PROPERTY CXX_STANDARD 17)
