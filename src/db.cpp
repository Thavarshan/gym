/**
 * @file db.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundamentals Assignment).
 * @version 2.5.5
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/db.h"
#include "include/details.h"
#include "include/utils.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Load all membership package and supplement related data into application.
 *
 * @param dataFile
 */
void loadPackageData(std::string dataFile)
{
    // First we define variables which will be used to parse and
    // split data from the csv files where data is stored in.
    std::string line, records;

    // We then read the csv files and store then in string format.
    records = readFile(dataFile); // This function is found in "utils.cpp"

    // We then convert the data into a string stream so it can then
    // be parsed into the format we require the data to be in.
    std::istringstream recordStream(records);

    // We now iterate through the stream line by line.
    while (std::getline(recordStream, line))
    {
        // Each line contains data that is separated by commas, so we split the
        // string into single string components using the commas.
        std::vector<std::string> result = explode(line, ','); // This function is found in "utils.cpp"

        // The first peice of string is the ID followed by the name and price.
        // Each peice will be split and saved into a structure specifically
        // made for packages. This will then be used as a lookup table.
        for (size_t i = 0; i < result.size(); i++)
        {
            // The appropriate peices will be saved into
            // the structure indexed by the package ID.
            struct Package record; // This is declared in "details.h"
            record.name = result[1];
            record.price = result[2];

            // Each individual data entry is then stores into
            // the package registry for reference later.
            registry.insert(std::make_pair(result[0], record));
        }
    }
}

/**
 * @brief Load all authenticatable user related data into application.
 *
 * @param dataFile
 */
void loadUserData(std::string dataFile)
{
    // First we define variables which will be used to parse and
    // split data from the csv files where data is stored in.
    std::string line, records;

    // We then read the csv files and store then in string format.
    records = readFile(dataFile); // This function is found in "utils.cpp"

    // We then convert the data into a string stream so it can then
    // be parsed into the format we require the data to be in.
    std::istringstream recordStream(records);

    // We now iterate through the stream line by line.
    while (std::getline(recordStream, line))
    {
        // Since each line contains data separated by commas we split the string
        // using the commas and each component will be stored accordingly.
        std::vector<std::string> result = explode(line, ','); // This function is found in "utils.cpp"

        for (size_t i = 0; i < result.size(); i++)
        {
            // The first peice of data is the user's email address followed by the
            // password that must be used to validate the authentication attempt.
            users.insert(std::make_pair(result[0], result[1]));
        }
    }
}

/**
 * @brief Load all data into application.
 */
void loadData()
{
    try
    {
        // First we load in the package and supplement related data.
        loadPackageData("./details/packages.csv");
    }
    catch (const std::exception &e)
    {
        // If something fails a message is displayed indicating
        // the point of failiure in the application.
        std::cout << "Failed to load package data.";
    }

    try
    {
        // We do the same and load the data for users that are
        // allowed to authenticate into the appkcaition.
        loadUserData("./details/users.csv");
    }
    catch (const std::exception &e)
    {
        // Failiure will display a message to indicate where is failed.
        std::cout << "Failed to load user data.";
    }
}

/**
 * @brief Delete structures that were set up to store package information.
 */
void terminateDatabase()
{
    // We must clear out the map data structures we used
    // to store package and user related information.
    registry.clear();
    users.clear();
}
