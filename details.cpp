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
