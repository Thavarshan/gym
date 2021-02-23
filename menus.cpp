#include "menus.h"
#include <cstdio>
#include <iostream>

/**
 * @brief Show application menu for user to choose what to do next.
 *
 * @return int
 */
int displayMainMenu()
{
    int option;

    printf("\n");
    printf("What would you like to do?\n");
    printf("<1> View fitness package details.\n");
    printf("<2> View food supplement details.\n");
    printf("<3> Purchase packages.\n");
    printf("<4> More about Rathnayaka Gyms.\n");
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
}
