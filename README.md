# Rathnayake Gym Application

## Introduction

Rathnayaka GYMS is a fitness center that has been grown rapidly throughout the country. They provide a quality healthcare service and give their members control over their health is paramount at all Rathnayaka GYMS. They help members prevent and overcome degenerative diseases, achieve their optimum fitness goals, realize personal lifestyle development objectives, and rehabilitate them into good health. This is accomplished by designing exercise programs that are effective, efficient, and motivational.

Due to the increasing number of members, they have planned to automate their billing process. Imagine that you are a software developer in this project and design and develop a system for Rathnayaka GYMS.

#### Basic functions of the app

- View fitness package details
- View food supplement details
- Calculate bill for invoice
- Information about business

## Usage

Please download the latest release of the application form [here](https://github.com/Thavarshan/gym-app/releases/download/v1.4.2/app.zip). Once downloaded you should find a directory named `app`. Inside the `app` directory you will find the following executables and directories.

- app - Unix executable.
- app.exe - Windows executable.
- invoices - Directory where invoices generated after each purchase is stored.
- details - Details regarding packages and business are stored and accessed by the app.

#### How to use the app

If you are on a Windows machine you can directly double click on and open the `app.exe` file and the application should startup on a terminal screen. It is recommended that the `app.exe` be run through a `terminal` app like `Powershell` on Windows. If you are on a Linux or Mac machine you need to access the `app` directory through your `terminal` application and one the `app` executable file through the `treminal` application by running:

```shell
./app
```

> Please make sure that the `details` and `invoices` directories are located right next to both `app` and `app.exe` executables inside the `app` directory. These directories are essential for the application to function properly.

Once the application is run you should be able to see a menu with a list of actions for you to choose from and an input prompting for your action choice. Type in the relative number associated with the action you would like to perform on to the prompt and press `enter`.

Upon choice of an action relating to "view details", the application will display relevant details on the screen. These details are accessed from the `details` directory located next to the executable file itself.

If option no. **4** is chosen you will be redirected to another prompt asking for your choice of a membership package to purchase. This is followed by another prompt asking how many packages you would like to purchase and yet another similar prompt asking if you would like to purchase any supplement packages to accompany your purchase of a membership package.

Please provide valid `package IDs` on both instances you are prompted for a choice otherwise the prompt will loop back and ask again until a valid `package ID` is provided.

Once the package and supplements are purchased the application will ask for your name. You can provide your full name with spaces included and the app will capture it as a whole string.

Finally, the bill amount and relevant charges are calculated and an invoice is generated and displayed back to you.

The invoice is also written to a file and saved within the `invoices` directory for use by the business.

#### How To Inspect The Source Code

The source code follows the below directory structure:

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

> Source code has multiple lines between code block to improve readability.

##### Useful Links

1. [Source code on `Github`](https://github.com/Thavarshan/gym1.app)
2. [CI/CD actions](https://github.com/Thavarshan/gym-app/actions)
3. [Releases](https://github.com/Thavarshan/gym-app/releases)

## Development

The project (not the app) is only installable through `git` and should be cloned from the remote repository. To clone the project to your local development environment or even production environment, run the below `shell` command through your terminal.

```shell
git clone git@github.com:Thavarshan/gym-app.git gym
```

You should see a directory called `gym` or `gym-app` at the place where you cloned the project.

All coding was done by [**Thavarshan Thayananthajothy**](mailto:tjthavarshan@gmail.com) (CL/HDCSE/95/15). No existing applications were used as a reference and extra knowledge on how to accomplish certain tasks was acquired through online research. The application source code follows `C++17` standards and `clang-format` coding standards.

### Tools & IDE

- IDE/Code Editor - [Visual Studio Code](https://code.visualstudio.com/)
- Build system - [CMake](https://cmake.org/)
- Compiler - [Clang (Mac)](https://clang.llvm.org/) & [MinGW-w64 (Windows)](http://mingw-w64.org/doku.php)
- Testing framework - [GoogleTest](https://github.com/google/googletest)
- Terminal application - [MacOS Terminal](https://iterm2.com/) & [Windows Terminal](https://www.microsoft.com/en-us/p/windows-terminal/9n0dx20hk701?activetab=pivot:overviewtab)
- Extra libraries - [libfort](https://github.com/seleznevae/libfort)

### Unit Testing

All unit testing both locally and on the cloud through CI was accomplished through [GoogleTest](https://github.com/google/googletest). [CMake](https://cmake.org/) was used to compile and test the executables and libraries.

### Continuous Integration (CI)

#### What is CI?

Continuous integration is a coding philosophy and set of practices that drive development teams to implement small changes and check-in code to version control repositories frequently (IDG Communications, Inc. 2020).

#### Implementation

The free service of CI provided by [Github](https://github.com/) was utilized for the CI pipeline. Every time code is pushed to `Github` a special automated action is run where a cloud droplet / virtual machine hosted on the cloud is propagated and is used to build and test the code in different platforms.

The code is compiled, built, and tested on `Linux`, `Mac` and `Windows` platforms. You can view all CI processes [here](https://github.com/Thavarshan/gym-app/actions).

Three services are run every time the code is pushed to `Github`.

1. Build and test application
2. Build application for release (normal use)
3. Static code analysis.

### Code Analysis

#### What is Static Code Analysis

Static code analysis is a method of debugging by examining source code before a program is run. It’s done by analyzing a set of code against a set (or multiple sets) of coding rules.

#### Implementation

As mentioned previously static code analysis service is automatically run by the CI pipeline every time updates to the source code are pushed to `Github`.

### Releases

Similar to static code analysis service the CI pipeline also performs an action where the application on successful testing builds for release or regular usage by end-user. The service produces usable executable files for both `Unix` and `Windows` platforms.

#### Semantic Versioning

Semantic versioning is a formal convention for specifying compatibility using a three-part version number: major version; minor version; and patch.

The application is released based on semantic versioning and is available for download [here](https://github.com/Thavarshan/gym-app/releases/download/v1.4.2/app.zip).

> This application and project as a whole was developed and is maintained by [**Thavarshan Thayananthajothy**](mailto:tjthavarshan@gmail.com).
