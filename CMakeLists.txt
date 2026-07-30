cmake_minimum_required(VERSION 3.25)
project(gym_cli VERSION 3.0.0 LANGUAGES CXX)

option(GYM_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(GYM_BUILD_TESTS "Build tests" ON)
option(GYM_ENABLE_CLANG_TIDY "Run clang-tidy during compilation" OFF)
option(GYM_ENABLE_COVERAGE "Instrument targets for gcov coverage" OFF)
option(GYM_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer" OFF)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(GYM_ENABLE_CLANG_TIDY)
  find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy REQUIRED)
  set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXECUTABLE};--warnings-as-errors=*)
endif()

find_package(SQLite3 REQUIRED)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
  pkg_check_modules(SODIUM QUIET IMPORTED_TARGET libsodium)
endif()

if(TARGET PkgConfig::SODIUM)
  set(GYM_SODIUM_TARGET PkgConfig::SODIUM)
else()
  find_package(unofficial-sodium CONFIG REQUIRED)
  set(GYM_SODIUM_TARGET unofficial-sodium::sodium)
endif()

if(TARGET SQLite3::SQLite3)
  set(GYM_SQLITE_TARGET SQLite3::SQLite3)
else()
  set(GYM_SQLITE_TARGET SQLite::SQLite3)
endif()

add_library(gym_domain src/domain.cpp)
target_include_directories(gym_domain PUBLIC src/include)

add_library(gym_infrastructure src/infrastructure.cpp)
target_include_directories(gym_infrastructure PUBLIC src/include)
target_link_libraries(gym_infrastructure PUBLIC gym_domain ${GYM_SQLITE_TARGET} ${GYM_SODIUM_TARGET})

add_library(gym_application src/application.cpp)
target_include_directories(gym_application PUBLIC src/include)
target_link_libraries(gym_application PUBLIC gym_domain gym_infrastructure)

add_library(gym_tui src/tui.cpp)
target_include_directories(gym_tui PUBLIC src/include)
target_link_libraries(gym_tui PUBLIC gym_application)

add_executable(gym src/main.cpp)
target_link_libraries(gym PRIVATE gym_tui)
set_target_properties(gym PROPERTIES OUTPUT_NAME gym)

foreach(target gym_domain gym_infrastructure gym_application gym_tui gym)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /permissive-)
    target_compile_definitions(${target} PRIVATE _CRT_SECURE_NO_WARNINGS)
    if(GYM_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE /WX)
    endif()
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion)
    if(GYM_WARNINGS_AS_ERRORS)
      target_compile_options(${target} PRIVATE -Werror)
    endif()
  endif()
endforeach()

include(CTest)
if(BUILD_TESTING AND GYM_BUILD_TESTS)
  add_executable(gym_tests tests/v3_tests.cpp)
  target_link_libraries(gym_tests PRIVATE gym_application)
  add_test(NAME gym_tests COMMAND gym_tests)
endif()

if(GYM_ENABLE_COVERAGE)
  if(MSVC)
    message(FATAL_ERROR "Coverage instrumentation is currently supported only on GCC and Clang")
  endif()
  foreach(target gym_domain gym_infrastructure gym_application gym_tui gym gym_tests)
    if(TARGET ${target})
      target_compile_options(${target} PRIVATE --coverage -O0 -g)
      target_link_options(${target} PRIVATE --coverage)
    endif()
  endforeach()
endif()

if(GYM_ENABLE_SANITIZERS)
  if(MSVC)
    message(FATAL_ERROR "Sanitizer instrumentation is currently supported only on Clang/GCC")
  endif()
  foreach(target gym_domain gym_infrastructure gym_application gym_tui gym gym_tests)
    if(TARGET ${target})
      target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
      target_link_options(${target} PRIVATE -fsanitize=address,undefined)
    endif()
  endforeach()
endif()

set(GYM_FORMAT_SOURCES
  src/application.cpp
  src/domain.cpp
  src/infrastructure.cpp
  src/main.cpp
  src/tui.cpp
  src/include/gym/application.hpp
  src/include/gym/domain.hpp
  src/include/gym/infrastructure.hpp
  src/include/gym/sqlite.hpp
  src/include/gym/tui.hpp
  tests/v3_tests.cpp)

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format)
if(CLANG_FORMAT_EXECUTABLE)
  add_custom_target(format
    COMMAND ${CLANG_FORMAT_EXECUTABLE} -i ${GYM_FORMAT_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Formatting C++ sources")

  add_custom_target(format-check
    COMMAND ${CLANG_FORMAT_EXECUTABLE} --dry-run --Werror ${GYM_FORMAT_SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Checking C++ source formatting")
else()
  message(STATUS "clang-format not found; format targets are unavailable")
endif()

include(GNUInstallDirs)
install(TARGETS gym RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES LICENSE README.md DESTINATION ${CMAKE_INSTALL_DATADIR}/gym-cli)

set(CPACK_PACKAGE_NAME "gym-cli")
set(CPACK_PACKAGE_VENDOR "Gym CLI")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_FILE_NAME "gym-cli-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CPACK_GENERATOR "ZIP;TGZ")
include(CPack)
