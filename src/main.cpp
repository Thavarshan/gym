/**
 * @file main.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundamentals Assignment).
 * @version 2.6.2
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/auth.h"
#include "include/bill.h"
#include "include/db.h"
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
    // We first load all the necessary data for the application. This data is
    // loaded from "build/details/packages.csv" and "build/details/users.csv".
    // This will act as our database.
    loadData(); // This function is found in "db.cpp"

    // We then define a variable to store the user's choice and another to
    // indicate whether the user has been authenticated in to the app.
    int choice = 0;
    bool authenticated = false;

    // Then, we display a welcome message to the user.
    displayMessage("welcome"); // This function is found in "menus.cpp"

    // Authentication menu on loop (wait for user to enter correct credentials).
    do
    {
        // We ask for the user's email address and password, then run them through
        // some checks to see if they are valid and authorized to give access to.
        // If they are, we log the user in to the application.
        authenticated = login(); // This function is found in "auth.cpp"
    } while (!authenticated);

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
        else if (choice == 5)
        {
            // If the user chooses to exit the appliction by choosing no. 5, we
            // break the loop and end the execution of the program.
            break;
        }
        else
        {
            // Otherwise, we get the details of packages, supplements, and about details.
            getDetails(choice); // This function is found in "details.cpp"
        }

        // We loops back and asks for the user input given that the user choice is within bounds.
    } while (true); // This function is found in "utils.cpp"

    // We will log the user out before we exit the application.
    authenticated = false;

    // We also have to release the memory we used to store the application data.
    terminateDatabase(); // This function is found in "db.cpp"

    // We finally thank the user and end the execution of the program.
    displayMessage("thankyou"); // This function is found in "menus.cpp"

    return 0;
}
