/**
 * @file utils.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundamentals Assignment).
 * @version 2.6.2
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/utils.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Accept only a numeric value from the user.
 *
 * @return int
 */
int valideInputNumber()
{
    // We define a variable that will hold an integer value.
    int option;

    // We then ask the user for an integer value.
    std::cin >> option;

    // We determine if the given input is a numeric value and
    // until the requirement is satisfied we will
    // repeatedly ask for a correct input.
    while (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry. Please enter a number: ";
        std::cin >> option;
    }

    // Only when the type is correct we return the input value.
    return option;
}

/**
 * @brief Split a string by a string.
 *
 * @param text
 * @param delimeter
 * @return std::vector<std::string>
 */
std::vector<std::string> explode(const std::string &text, const char &delimeter)
{
    std::string next;
    std::vector<std::string> result;

    // For each character in the string
    for (std::string::const_iterator iterator = text.begin(); iterator != text.end(); iterator++)
    {
        // If we've hit the terminal character
        if (*iterator == delimeter)
        {
            // If we have some characters accumulated
            if (!next.empty())
            {
                // Add them to the result vector
                result.push_back(next);
                next.clear();
            }
        }
        else
        {
            // Accumulate the next character into the sequence
            next += *iterator;
        }
    }

    if (!next.empty())
    {
        result.push_back(next);
    }

    return result;
}

/**
 * @brief Ask name from the user.
 *
 * @return std::string
 */
std::string askName()
{
    // This is where we will save the user's input.
    std::string name;

    // We ask the user for their name (full-name).
    printf("\n");
    printf("Please type in the customer\'s full name: ");
    // Since we are asking for the full-name of the user and it contains spaces
    // in between, we use the below code to incorporate the spaces.
    std::cin.ignore();
    std::getline(std::cin, name);

    return name;
}

/**
 * @brief Determine if the given element is in the list.
 *
 * @param element
 * @param list
 * @return true
 * @return false
 */
const bool inList(std::string element, std::vector<std::string> &list)
{
    // This checks if the given element is in the given vector.
    return (std::find(list.begin(), list.end(), element) != list.end());
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
    // This should give us the current date/time based on the current system.
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

    // We then determine if opening the file was successful
    // and save the status (true/false) to a variable.
    const bool status = file.good();

    // We do some cleaning up.
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
    // We first try to open a given file, and if it does not
    // exist we will create it using the name provided.
    std::ofstream writefile(file);

    // We then write th given contents to the file.
    writefile << content << std::endl;

    // We need to make sure to close the file system service.
    writefile.close();
}

/**
 * @brief Read content of given file and display it for the user.
 *
 * @param file
 */
std::string readFile(std::string file)
{
    std::string data;

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
        // If it is, we read the contents from the file and save it to a variable.
        std::ostringstream stringStream;
        stringStream << dataFile.rdbuf();
        data = stringStream.str();
    }

    // Again we need to make sure to close the file system service.
    dataFile.close();

    return data;
}
