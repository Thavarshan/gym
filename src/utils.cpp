/**
 * @file utils.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.4.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/utils.h"
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
    // This is where we will save the user's input.
    std::string name;

    // We ask the user for their name.
    printf("\n");
    printf("Please type in your name: ");
    // Since we are asking for the full-name of the user and it contains spaces in between
    // we use the below code to incorporate the spaces.
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
    // This is an old trick we use to find if the given value is within range.
    return ((input - low) <= (high - low));
}

/**
 * @brief Get current date and time information.
 *
 * @return std::string
 */
std::string currentDateTime()
{
    // This should give us the current date/time based on current system.
    time_t now = time(0);

    // We then convert it to string format.
    std::string dateTime = ctime(&now);

    return dateTime;
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

    // We then determine if opening the file was successful and save the
    // status (true/false) to a variable.
    bool status = file.good();

    // We don some cleaning up.
    file.close();

    // Finally we return the status of our operation.
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
    // We first try to open a given file, and if it does not exist we will create it
    // using the name provided.
    std::ofstream writefile(file);

    // We then write the contents to the file.
    writefile << content << std::endl;

    // We need to make sure to close the file system service.
    writefile.close();
}

/**
 * @brief Read content of given file and display it for the user.
 *
 * @param file
 */
void readFile(std::string file)
{
    // We first determine whether the given file exists.
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

    // Again we need to make sure to close the file system service.
    dataFile.close();
}
