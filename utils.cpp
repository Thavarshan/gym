#include "utils.h"
#include <fstream>
#include <iostream>

bool isWithinRange(unsigned low, unsigned high, unsigned input)
{
    return ((input - low) <= (high - low));
}

bool fileExists(const char *name)
{
    std::ifstream file(name);

    return file.good();
}

void readFile(const char *file)
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
