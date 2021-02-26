/**
 * @file menu.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

/**
 * @file menus.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "includes/menus.h"
#include "includes/details.h"
#include <iostream>
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

    return option;
}

/**
 * @brief Show menu for user to choose a purchase from.
 *
 * @return std::map<std::string, int>
 */
std::map<std::string, int> displayPurchaseMenu()
{
    std::string package, supplement;
    int pkgUnits, splUnits = 0;

    printf("\n");
    printf("Which membership package would you like to prchase? ");
    printf("\n");
    getDetails(1);
    printf("Type in the package ID: ");
    std::cin >> package;
    printf("How many would you like to purchase: ");
    std::cin >> pkgUnits;

    printf("\n");
    printf("Would you like to prchase a supplement package to go along with your membership? ");
    printf("\n");
    getDetails(2);
    printf("Type in the supplements package ID (if you do not wish to purchase this item, please type in 'no'): ");
    std::cin >> supplement;

    if (supplement != "No" && supplement != "no")
    {
        printf("How many would you like to purchase: ");
        std::cin >> splUnits;
    }
    else
    {
        supplement = "N/A";
    }

    std::map<std::string, int> choices{{package, pkgUnits}, {supplement, splUnits}};

    return choices;
}
