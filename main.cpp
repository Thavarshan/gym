/**
 * @file main.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Invoice Generator (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "bill.h"
#include "details.h"
#include "menus.h"
#include <iostream>

/**
 * @brief Execute the program.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const *argv[])
{
    int choice;

    printf("Welcome to Rathnayaka Gym.\n");

    do
    {
        choice = displayMainMenu();

        getDetails(choice);
    } while (choice <= 4);

    printf("Thank you for visiting us! Please come again.\n");

    return 0;
}
