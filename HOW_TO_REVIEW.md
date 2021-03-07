# How To Inspect The Source Code

The source code follows the below directory structure

```
.
└── sourcecode/
    ├── src/
    │   ├── includes/
    │   │   ├── bill.h
    │   │   ├── menus.h
    │   │   ├── invoice.h
    │   │   ├── details.h
    │   │   ├── fort.h
    │   │   └── fort.hpp
    │   ├── main.cpp <- Start here!
    │   ├── menus.cpp
    │   ├── invoice.cpp
    │   ├── details.cpp
    │   ├── bill.cpp
    │   └── fort.cpp
    ├── tests/
    │   ├── bill_test.cpp
    │   ├── details_test.cpp
    │   ├── util_test.cpp
    │   ├── main.cpp
    │   └── CMakeLists.txt
    ├── googletest
    └── CMakeLists.txt
```

When inspecting the code please start with the `src` directory and follow the below order.

1. `main.cpp`
2. `menus.cpp`
3. `details.cpp`
4. `bill.cpp`
5. `utils.cpp`
6. `invoice.cpp`

> The relevant header files are located within the `include` directory inside the `src` directory.

The source code is also stored on [`Github`](https://github.com/) at [`Thavarshan/gym-app`](https://github.com/Thavarshan/gym-app) for storage, version management and CI/CD purposes. All unit test files are found within the `tests` directory and the testing framework itself is found inside the `googletest` directory.

Please note that an external library called [`libfort`](https://github.com/seleznevae/libfort) is also included within the `src` directory in the files:

1. `fort.cpp`
2. `include/fort.h`
3. `include/fort.hpp`

## Useful Links

1. [Source code on `Github`](https://github.com/Thavarshan/gym1.app)
2. [CI/CD actions](https://github.com/Thavarshan/gym-app/actions)
3. [Releases](https://github.com/Thavarshan/gym-app/releases)
