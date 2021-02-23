#include "menus.h"
#include <cstdio>
#include <iostream>

int displayMainMenu()
{
    int option;

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

int displayPurchaseMenu()
{
    printf("Get ready to implress!\n");

    return 0;
}
