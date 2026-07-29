# Contributing to Gym CLI

Thanks for helping improve Gym CLI. Contributions of code, documentation, design feedback, tests, and issue triage are all welcome.

## Before opening an issue

- Search existing issues and discussions first.
- Use Discussions for support, ideas, and broad design questions.
- Use the security reporting path in [SECURITY.md](SECURITY.md) for vulnerabilities.
- Use an issue form only for reproducible defects or a scoped enhancement.

## Development workflow

1. Fork the repository and create a focused branch from `main`.
2. Build and test locally:

   ```sh
   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
   cmake --build build --parallel
   ctest --test-dir build --output-on-failure
   ```

3. Format changed C++ files with `cmake --build build --target format`.
4. Verify formatting without changing files using `cmake --build build --target format-check`.
5. Add or update tests for every behavior change.
6. Open a pull request using the repository template and link the relevant issue.

## Engineering expectations

- Keep domain rules independent of persistence and terminal UI concerns.
- Prefer small, cohesive changes with explicit error handling and tests.
- Do not add credentials, databases, invoices, generated binaries, or personal data to the repository.
- Preserve the C++23 baseline and build with warnings enabled; CI treats warnings as errors.
- Use clear, inclusive language and document user-visible behavior.

## Pull request process

- Keep one logical change per pull request.
- Explain the problem, approach, test evidence, and any operational impact.
- Update documentation and changelog entries when appropriate.
- All required CI, CodeQL, dependency-review, and maintainer review checks must pass before merge.
- Maintainers use squash merges and delete merged branches.

By contributing, you agree to follow the [Code of Conduct](CODE_OF_CONDUCT.md).
