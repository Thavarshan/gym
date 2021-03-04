/**
 * @file main.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.3.5
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/bill.h"
#include "include/details.h"
#include "include/invoice.h"
#include "include/menus.h"
#include "include/utils.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

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
    int choice = 0;

    // Then we display a welcome message to the user.
    printf("Welcome to Rathnayaka GYMS.\n");

    // Menu on loop (wait for user to choose an option).
    do
    {
        // We ask for what the user wants to do given the options.
        choice = displayMainMenu(); // This function is found in "menus.cpp"

        if (choice == 4)
        {
            // As a special case of the user's choice, if the user chooses the 4th option
            // we redirect them to the purchase module.
            std::map<std::string, int> choices = displayPurchaseMenu(); // This function is found in "menus.cpp"

            // Using the package and supplement choices the user made we calculate the final bill.
            std::map<std::string, double> details = makePurchase(choices); // This function is found in "bill.cpp"

            // Then using the bill details we generate an invoice and display it to the user.
            generateInvoice(details); // This function is found in "invoice.cpp"
        }
        else
        {
            // Otherwise we get the details of packages, supplements and about details.
            getDetails(choice); // This function is found in "details.cpp"
        }

        // We loops back and asks for the user input given that the user choice is within bounds.
    } while (inRange(choice, 0, 4)); // This function is found in "utils.cpp"

    // We finally thank the user and end the execution of the program.
    printf("Thank you for visiting us! Please come again.\n");

    return 0;
}
