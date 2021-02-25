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
#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Lookup table for details available about the business.
 *
 * @param index
 * @return std::string
 */
std::string details(int index)
{
    std::vector<std::string> details{
        "./details/instructions.txt",
        "./details/packages.txt",
        "./details/supplements.txt",
        "./details/about.txt",
    };

    return details[index];
}

/**
 * @brief Determine if the given string is a valid package ID.
 *
 * @param id
 * @return true
 * @return false
 */
bool isPackage(std::string id)
{
    std::vector<std::string> packages{
        "N/A",
        "PKGDT001",
        "PKGDT002",
        "PKGDT003",
        "PKGDT004",
        "PKGDT005",
        "ITMP001",
        "ITMB002",
        "ITMB003",
        "ITMF004",
        "ITMC005",
        "ITMS006",
        "ITMG007",
    };

    if (std::find(packages.begin(), packages.end(), id) != packages.end())
    {
        return true;
    }
    else
    {
        return false;
    }
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
        {"N/A", 0.00},
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
 * @brief Determine the name of a given membership/supplement package.
 *
 * @param id
 * @return std::string
 */
std::string packageNameLookup(std::string id)
{
    std::map<std::string, std::string> packages{
        {"N/A", "No supplements"},
        {"PKGDT001", "DAY WORKOUT"},
        {"PKGDT002", "EXECUTIVE MEM(FULL)"},
        {"PKGDT003", "1 MONTH MEMBERSHIP"},
        {"PKGDT004", "MASTERS MEMBERSHIP OVER 60 Yrs"},
        {"PKGDT005", "NON-PEAK EXECUTIVE"},
        {"ITMP001", "PLATINUM (100% CREATINE)"},
        {"ITMB002", "BEAST AMINOLYTES"},
        {"ITMB003", "BEAST SUPER SAUNA"},
        {"ITMF004", "ON FISH OIL (100 SOFT-GELS)"},
        {"ITMC005", "CREATINE 5000 (60 SERVINGS)"},
        {"ITMS006", "D-STUNNER ALPHA (30 SERVINGS)"},
        {"ITMG007", "GOLD STANDARD GAINER 10LBS"},
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
