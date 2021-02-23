/**
 * @file main.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "bill.h"
#include "details.h"
#include "menus.h"
#include "utils.h"
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
    // We first define a variable to store the user's choice in.
    int choice;

    // Then we display a welcome message to the user.
    printf("Welcome to Rathnayaka Gyms.\n");

    // Menu on loop.
    do
    {
        // We ask for what the user wants to do given the options.
        choice = displayMainMenu();

        if (choice == 4)
        {
            // As a special case of the user's choice, if the user chooses the 4th option
            // we redirect them to the purchase module.
            displayPurchaseMenu();
        }
        else
        {
            // Otherwise we get the details of packages, supplements and about details.
            getDetails(choice);
        }

        // We loops back and asks for the user input given that the user choice is within bounds.
    } while (inRange(choice, 0, 4));

    // We finally thank the user and end the execution of the program.
    printf("Thank you for visiting us! Please come again.\n");

    return 0;
}
