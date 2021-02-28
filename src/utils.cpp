/**
 * @file utils.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "includes/utils.h"
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
    std::cin.ignore();
    std::getline(std::cin, name);

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
const bool inRange(unsigned low, unsigned high, unsigned input)
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
const bool fileExists(std::string name)
{
    // We try to open a given file.
    std::ifstream file(name);

    // We determine if opening the file was successful.
    bool status = file.good();

    // Cleaning up.
    file.close();

    // We return the status of our operation.
    return status;
}

/**
 * @brief Write (append) given content to txt file.
 *
 * @param file
 * @param content
 */
void writeFile(std::string file, std::string content)
{
    // We first determine whether this file exists.
    if (!fileExists(file))
    {
        // If not we throw an exception and end execution.
        throw std::runtime_error("File does not exist.");
    }

    // If it does, we open it with the appropriate flags to append and not overrite the content within.
    std::ofstream writefile(file, std::ios::out | std::ios::app);

    // We write the content to the file.
    writefile << content << std::endl;

    // Make sure to close the FS service.
    writefile.close();
}

/**
 * @brief Read content of given file and display it for the user.
 *
 * @param file
 */
void readFile(std::string file)
{
    // We first determine whether this file exists.
    if (!fileExists(file))
    {
        // If not we throw an exception and end execution.
        throw std::runtime_error("File does not exist.");
    }

    // If it does, we open the file to be read from.
    std::ifstream dataFile(file);

    // We determine whether the file is open and is accessible.
    if (dataFile.is_open())
    {
        // If it is, we read the contents from the file and display it to the screen.
        std::cout << dataFile.rdbuf() << std::endl;
    }

    // Make sure to close the FS service.
    dataFile.close();
}

/**
 * @brief Create a file and write the given contents to the  file.
 *
 * @param file
 * @param content
 */
void createAndWriteToFile(std::string file, std::string content)
{
    // Create new file.
    std::ofstream outfile(file);

    // Write contents to created file.
    outfile << content << std::endl;

    // Cleanup after.
    outfile.close();
}
