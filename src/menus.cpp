/**
 * @file menus.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.4.2
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/menus.h"
#include "include/details.h"
#include <iostream>
#include <limits>
#include <map>
#include <string>

/**
 * @brief Show application menu for user to choose what to do next.
 *
 * @return int
 */
int displayMainMenu()
{
    // We define a variable to store the user's choice.
    int option;

    // We then display a list of choices the user can make.
    printf("\n");
    printf("What would you like to do?\n");
    printf("<1> View fitness package details.\n");
    printf("<2> View food supplement details.\n");
    printf("<3> More about Rathnayaka Gyms.\n");
    printf("<4> Purchase packages.\n");
    printf("<5> Exit application.\n");
    printf("\n");

    printf("Please choose an option: ");
    std::cin >> option;

    while (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Bad entry.  Enter a NUMBER: ";
        std::cin >> option;
    }

    // The option will be an index that we use to choose which details should be retrieved from
    // a vector of elements containing files with package details as its content.
    return option;
}

/**
 * @brief Show menu for user to choose a purchase from.
 *
 * @return std::map<std::string, int>
 */
std::map<std::string, int> displayPurchaseMenu()
{
    // We first define variables to store the user's package and supplement choices
    // along with how many units of each they would like to purchase.
    std::string package, supplement;
    int pkgUnits, splUnits = 0;

    // We then ask them their choice by displaying the package details to choose
    // from and ask them to type in the relevant package ID.
    printf("\n");
    printf("Which membership package would you like to prchase? ");
    printf("\n");
    getDetails(1); // This function is found in "details.cpp"

    do
    {
        // We can only allow a valid package ID to be entered by the user.
        // So, until a valid package ID is provided the request
        // will be asked again and again in a loop.
        printf("Type in the package ID: ");
        std::cin >> package;
    } while (!isPackage(package)); // This function is found in "details.cpp"

    printf("How many would you like to purchase: ");
    std::cin >> pkgUnits;

    // Now we ask for the supplement package choice.
    printf("\n");
    printf("Would you like to prchase a supplement package to go along with your membership? ");
    printf("\n");
    getDetails(2); // This function is found in "details.cpp"

    do
    {
        // Again, to make sure the user types in a valid supplement package ID
        // we keep asking again and again until a valid ID is entered.
        printf("Type in the supplements package ID (if you do not wish to purchase this item, please type in 'no'): ");
        std::cin >> supplement;
    } while (!isPackage(supplement)); // This function is found in "details.cpp"

    // We now do some cleanup by setting acceptable values to the variables.
    if (supplement != "No" && supplement != "no")
    {
        // If the user chooses an actual supplement package,
        // we will proceed by asking them how many
        // units they would like to purchase.
        printf("How many would you like to purchase: ");
        std::cin >> splUnits;
    }
    else
    {
        // In the case the user does not want to purchase a supplement package we set
        // the supplement package ID to "N/A" (null purchase).
        supplement = "N/A";
    }

    // We finally save the user's purchase choices within a map to be passed to the bill calculator module.
    std::map<std::string, int> choices{{package, pkgUnits}, {supplement, splUnits}};

    return choices;
}
