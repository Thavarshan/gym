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
#include <stdexcept>

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
