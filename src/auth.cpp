/**
 * @file auth.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundamentals Assignment).
 * @version 2.5.5
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/auth.h"
#include "include/details.h"
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

/**
 * @brief Authenticate user using the given email and password.
 *
 * @param credentials
 * @return true
 * @return false
 */
bool authenticate(std::map<std::string, std::string> &credentials)
{
    try
    {
        // We authenticate the user by checking if the given email address is in
        // the list if authorized personale allowed to use this application.
        std::string password = authUserLookup(credentials["email"]); // This function is found in "details.cpp"

        // We then run a secondary check to see if the password they have provided
        // is the one belonging to the email address saved in the list.
        if (password == credentials["password"])
        {
            return true;
        }

        return false;
    }
    catch (const std::exception &e)
    {
        return false;
    }
}

/**
 * @brief Log the current user in to the application.
 *
 * @return true
 * @return false
 */
bool login()
{
    // These variables will be used to store the user's email and
    // password inputs. As well as the combination of both so
    // they can be passed between modules as a pair.
    std::string email, password;
    std::map<std::string, std::string> credentials;

    printf("Please log in to continue.\n");
    printf("\n");

    printf("Enter your email address: ");
    std::cin >> email;
    printf("Enter your password: ");
    std::cin >> password;
    printf("\n");

    // We will save the user's inputs as a key, value pair
    // so we can distinguish between both credentials.
    credentials.insert(std::make_pair("email", email));
    credentials.insert(std::make_pair("password", password));

    // We will now try to validate the user input details and
    // attempt to authenticate the user into the application.
    return authenticate(credentials);
}
