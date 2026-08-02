# Development Guide

## Toolchain

Gym CLI requires CMake 3.25+, a C++23 compiler, SQLite3, and libsodium. CI uses the pinned vcpkg manifest for reproducible dependencies.

## Local commands

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
cmake --build build --target format-check
```

Use `cmake --build build --target format` only when you intend to rewrite source formatting.

## Quality checks

- `GYM_WARNINGS_AS_ERRORS=ON` promotes compiler warnings to errors.
- `GYM_ENABLE_SANITIZERS=ON` enables AddressSanitizer and UndefinedBehaviorSanitizer on supported compilers.
- `GYM_ENABLE_COVERAGE=ON` instruments targets for gcov/gcovr coverage reporting.
- `GYM_ENABLE_CLANG_TIDY=ON` enables the repository clang-tidy policy.

The GitHub workflow runs formatting, analysis, sanitizers, coverage, CodeQL, dependency review, and the full platform matrix. Run the narrowest relevant checks locally before opening a pull request.

## Tests

Tests must be deterministic, isolated from user data, and able to run in parallel CI workers. Use temporary directories for filesystem/database tests and avoid reliance on the current working directory, wall-clock timing, or network access.
