/**
 * @file db.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 2.5.4
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
    std::string line, records;
    records = readFile(dataFile);

    std::istringstream recordStream(records);

    while (std::getline(recordStream, line))
    {
        std::vector<std::string> result = explode(line, ',');

        for (size_t i = 0; i < result.size(); i++)
        {
            struct Package record;
            record.name = result[1];
            record.price = result[2];

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
    std::string line, records;
    records = readFile(dataFile);

    std::istringstream recordStream(records);

    while (std::getline(recordStream, line))
    {
        std::vector<std::string> result = explode(line, ',');

        for (size_t i = 0; i < result.size(); i++)
        {
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
        loadPackageData("./details/packages.csv");
    }
    catch (const std::exception &e)
    {
        std::cout << "Failed to load package data.";
    }

    try
    {
        loadUserData("./details/users.csv");
    }
    catch (const std::exception &e)
    {
        std::cout << "Failed to load user data.";
    }
}
