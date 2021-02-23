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
#include <iostream>

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
 * @return int
 */
int displayPurchaseMenu()
{
    printf("Get ready to implress!\n");

    return 0;
}
