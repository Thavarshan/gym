/**
 * @file details.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "utils.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

/**
 * @brief Lookup table for details available about the business.
 *
 * @param index
 * @return const char*
 */
const char *details(int index)
{
    const char *details[4] = {
        "./details/instructions.txt",
        "./details/packages.txt",
        "./details/supplements.txt",
        "./details/about.txt",
    };

    return details[index];
}

/**
 * @brief Determine the price of a given membership/supplement package.
 *
 * @param id
 * @return float
 */
float packagePriceLookup(std::string id)
{
    std::map<std::string, float> packages{
        {"PKGDT001", 1500.00},
        {"PKGDT002", 5500.00},
        {"PKGDT003", 3000.00},
        {"PKGDT004", 5000.00},
        {"PKGDT005", 3300.00},
        {"ITMP001", 4000.00},
        {"ITMB002", 6000.00},
        {"ITMB003", 6000.00},
        {"ITMF004", 3500.00},
        {"ITMC005", 3000.00},
        {"ITMS006", 6500.00},
        {"ITMG007", 20000.00},
    };

    return packages[id];
}

/**
 * @brief Get the details the user requested.
 *
 * @param choice
 */
void getDetails(int choice)
{
    try
    {
        readFile(details(choice));
    }
    catch (...)
    {
        printf("\n");
    }
}
