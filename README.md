# Gym CLI

[![Continuous integration](https://github.com/Thavarshan/gym/actions/workflows/ci.yml/badge.svg)](https://github.com/Thavarshan/gym/actions/workflows/ci.yml)
[![CodeQL](https://github.com/Thavarshan/gym/actions/workflows/codeql.yml/badge.svg)](https://github.com/Thavarshan/gym/actions/workflows/codeql.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Gym CLI is a secure, single-workstation gym administration application written in modern C++23. It manages members, membership plans, supplements, stock, tax-aware invoices, PDF receipts, and CSV operational reports without requiring a network service.

> Gym CLI v3 is a clean-start release. It intentionally does not import the earlier CSV application or its records.

## Highlights

- SQLite-backed data with migrations, foreign keys, WAL, and integrity checks.
- Argon2id password hashing through libsodium.
- Member, catalog, inventory, checkout, invoice-void, and reporting workflows.
- Immutable invoice snapshots, auditable inventory movements, PDF receipts, and CSV exports.
- Cross-platform CMake build, tests, formatting, sanitizers, CodeQL, and tag-driven releases.

## Quick start

### Requirements

- CMake 3.25 or newer
- A compiler with C++23 support
- SQLite3 and libsodium, or [vcpkg](https://learn.microsoft.com/vcpkg/)

### Build and test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

For a reproducible dependency environment, configure CMake with the vcpkg toolchain:

```sh
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DCMAKE_BUILD_TYPE=Debug
```

### Run

```sh
./build/gym
./build/gym doctor --json
```

The first launch creates a data directory, initializes the database, and guides you through creating the business profile and first administrator.

Use `--data-dir <path>` or `GYM_DATA_DIR` to choose a data location. See the [user guide](docs/USER_GUIDE.md) for operational details.

## Documentation

- [Architecture](docs/ARCHITECTURE.md)
- [Development guide](docs/DEVELOPMENT.md)
- [User guide](docs/USER_GUIDE.md)
- [Security policy](SECURITY.md)
- [Contributing guide](CONTRIBUTING.md)
- [Release process](docs/RELEASING.md)
- [Changelog](CHANGELOG.md)

## Community

- Use [GitHub Discussions](https://github.com/Thavarshan/gym/discussions) for questions, ideas, and community help.
- Use issue forms for reproducible bugs and well-scoped feature requests.
- Please read the [Code of Conduct](CODE_OF_CONDUCT.md) before participating.
- Security issues must follow the private reporting path in [SECURITY.md], not public issues.

## Backup and data ownership

Gym CLI does not make automatic backups. Close the app before using approved tooling to copy the complete data directory, including the SQLite database, WAL/SHM sidecars, invoices, and exports. Test restores in a separate data directory with `gym doctor`.

## License

Copyright © 2021–Present Thavarshan Thayananthajothy. Gym CLI is available under the [MIT License](LICENSE).
