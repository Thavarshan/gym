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

#include "menus.h"
#include "details.h"
#include <iostream>
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
 * @return struct
 */
int *displayPurchaseMenu()
{
    int choices[2] = {};

    printf("Which membership package would you like to prchase? ");
    getDetails(1);
    printf("Type in the package ID: ");
    std::cin >> choices[0];

    printf("Would you like to prchase a supplement package to go along with your membership? ");
    getDetails(2);
    printf("Type in the supplements package ID: ");
    std::cin >> choices[1];

    return choices;
}
