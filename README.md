# Rathnayake Gym Application

## Introduction

Rathnayaka GYMS is a fitness center that has been grown rapidly throughout the country. They provide a quality healthcare service and give their members control over their health is paramount at all Rathnayaka GYMS. They help members prevent and overcome degenerative diseases, achieve their optimum fitness goals, realize personal lifestyle development objectives, and rehabilitate them into good health. This is accomplished by designing exercise programs that are effective, efficient, and motivational.

Due to the increasing number of members, they have planned to automate their billing process. Imagine that you are a software developer in this project and design and develop a system for Rathnayaka GYMS.

#### Basic functions of the app
- View fitness package details
- View food supplement details
- Calculate bill
- Information about business.

## Installation

The project is only installable through `git` and should be cloned from the remote repository. To clone the project to your local development environment or even production environment, run the below `shell` command through your terminal.

```shell
git clone git@github.com:Thavarshan/gym-app.git gym
```

You should see a directory called `gym` at the place where you cloned the project.

## Usage

If the project does not contain an executable file named `gym` the application will have to be compiled. See below for compilation instructions.

### Mac/Linux Systems

If you are running a `Unix based system you can execute the application by running:

```shell
cd build
./app
```

### Windows Systems

On Windows systems run:

```shell
cd build
./app.exe
```

In both cases, a terminal window should open with a prompt asking you to choose an option.

> When running the executable `app` or `app.exe` make sure your **current-working-directory (cwd)** is located at the same place where the `details` directory.

## Development

After cloning the project, the basic project structure should contain a `src` directory and `tests` directory. The `src` directory contains all the source files of the application while the `tests` directory contains all relevant tests for the application.

#### Project Structure & Libraries

- All `header` files are located within the `src/includes` directory.
- Information regarding **membership packages** and **supplement details** are store and retrieved from within the `details` directory located in the project base path itself.
- This app uses a console text table generator `c++` library called [`libfort`](https://github.com/seleznevae/libfort) to generate invoice tables in the console window.

#### Compilation

##### Clang++

Use the following command when using `clang++` to compile the project.

```shell
/usr/bin/clang++ -std=c++17 -stdlib=libc++ -g -I/src/includes ./src/*.cpp -o ./build/gym
```

##### CMake

If you prefer to use `cmake` to build the application use the following command.

```shell
/usr/local/bin/cmake --build . --config Debug --target all -- -j 6
```

> This application and project as a whole was developed and is maintained by [**Thavarshan Thayananthajothy**](mailto:tjthavarshan@gmail.com).
