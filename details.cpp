#include <fstream>
#include <iostream>

/**
 * @brief Read content of ".txt" file and display it for the user.
 *
 * @param file
 */
void readFile(const char *file)
{
    std::ifstream dataFile(file);

    if (dataFile.is_open())
    {
        printf("\n");
        std::cout << dataFile.rdbuf() << std::endl;
        printf("\n");
    }
}

/**
 * @brief Lookup table for details available about the business.
 *
 * @param index
 * @return const char*
 */
const char *details(int index)
{
    const char *details[4] = {
        "details/instructions.txt",
        "details/packages.txt",
        "details/supplements.txt",
        "details/about.txt",
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
        readFile(details(0));
    }
}
