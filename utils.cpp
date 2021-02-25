/**
 * @file utils.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "utils.h"
#include <fstream>
#include <iostream>
#include <string>

/**
 * @brief Ask name from the user.
 *
 * @return std::string
 */
std::string askName()
{
    std::string name;

    printf("\n");
    printf("Please type in your name: ");
    std::cin >> name;

    return name;
}

/**
 * @brief Deterine if the given input is within acceptable range.
 *
 * @param low
 * @param high
 * @param input
 * @return true
 * @return false
 */
bool inRange(unsigned low, unsigned high, unsigned input)
{
    return ((input - low) <= (high - low));
}

/**
 * @brief Determine if the given file exists.
 *
 * @param name
 * @return true
 * @return false
 */
bool fileExists(std::string name)
{
    std::ifstream file(name);

    return file.good();
}

/**
 * @brief Read content of given file and display it for the user.
 *
 * @param file
 */
void readFile(std::string file)
{
    if (!fileExists(file))
    {
        throw std::runtime_error("File does not exist.");
    }

    std::ifstream dataFile(file);

    if (dataFile.is_open())
    {
        printf("\n");
        std::cout << dataFile.rdbuf() << std::endl;
        printf("\n");
    }

    dataFile.close();
}
