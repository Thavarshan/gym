/**
 * @file details.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.4.2
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/utils.h"
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
    // We will use this to show the relevant content to the user during the entrace menu part.
    // These are a list of file locations to relevant details.
    std::vector<std::string> details{
        "details/instructions.txt",
        "details/packages.txt",
        "details/supplements.txt",
        "details/about.txt",
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
const bool isPackage(std::string id)
{
    // A list of only IDs of all packages available to purchase including the ID of a "null" purchase.
    std::vector<std::string> packages{
        "no",
        "No",
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

    // We determine if the given ID is actually a valid package ID by looking it up on the above vector.
    return (std::find(packages.begin(), packages.end(), id) != packages.end());
}

/**
 * @brief Determine the price of a given membership/supplement package.
 *
 * @param id
 * @return double
 */
double packagePriceLookup(std::string id)
{
    // A list of package ID and its unit price.
    std::map<std::string, double> packages{
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
    // A list of package ID and its name.
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
    // We can only allow an existing index to be passed
    // in as the choice. e.x. 0, 1, 2, 3 only.
    try
    {
        // If the choice value is within the given range (0, 1, 2, 3) then we
        // get the name of the file the details are stored in, and retrieve
        // the file from the relevant location. Once the file is read the
        // content is printed out to the screen.
        readFile(details(choice)); // This function is found in "utils.cpp"
    }
    catch (...)
    {
        // In the case no valid choice is found we print out the
        // instaructions on how to properly use the application.
        readFile(details(0));
    }
}
