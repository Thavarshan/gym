/**
 * @file details.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka Gym Application (ICBT Batch 95 - Programming Fundamentals Assignment).
 * @version 2.6.2
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/details.h"
#include "include/utils.h"
#include <algorithm>
#include <cstdio>
#include <exception>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * @brief Package registry.
 */
std::map<std::string, struct Package> registry;

/**
 * @brief Authenticatable users.
 */
std::map<std::string, std::string> users;

/**
 * @brief Get the password of the given email address.
 *
 * @param email
 * @return std::string
 */
std::string authUserLookup(std::string email)
{
    // We first check if the given email is in the above list.
    if (users.find(email) != users.end())
    {
        // If it is, we return the password belonging to the email.
        return users[email];
    }

    // Otherwise we throw a logic error.
    throw std::runtime_error("Login failed.");
}

/**
 * @brief Lookup table for details available about the business.
 *
 * @param index
 * @return std::string
 */
std::string details(int index)
{
    // We will use this to show the relevant content to the user during the entrace menu part.
    // These are a list of file locations to relevant details.
    std::vector<std::string> details{
        "details/instructions.txt",
        "details/packages.txt",
        "details/supplements.txt",
        "details/about.txt",
    };

    return details[index];
}

/**
 * @brief Determine if the given string is a valid package ID.
 *
 * @param id
 * @return true
 * @return false
 */
const bool isPackage(std::string id)
{
    // We determine if the given ID is actually a valid package ID by looking it up on the package registry.
    return registry.find(id) != registry.end();
}

/**
 * @brief Lookup a specific detail about the package with the given ID.
 *
 * @param key
 * @param id
 * @return std::string
 */
std::string packageDetailLookup(std::string key, std::string id)
{
    // This variable will be used to store and return the requested data.
    std::string item;

    // We first check if the key they have provided is valid. Valid key's used by
    // the package structure are, "price" and "name".
    if (key == "price")
    {
        // We access the structure and save the value to the variable defined.
        item = registry[id].price;
    }
    else if (key == "name")
    {
        // We access the structure and save the value to the variable defined.
        item = registry[id].name;
    }
    else
    {
        // In the case an invalid key is provided be throw an error
        // pointing out exactly what the user did wrong.
        throw std::runtime_error("Invalid key provided");
    }

    // We also have to check if the provided key has any value within the registry.
    if (item.empty())
    {
        // If not we do the same and throw an error.
        throw std::runtime_error("Package ID is invalid");
    }

    return item;
}

/**
 * @brief Determine the price of a given membership/supplement package.
 *
 * @param id
 * @return double
 */
double packagePriceLookup(std::string id)
{
    // We will check and return package price details.
    return std::stod(packageDetailLookup("price", id));
}

/**
 * @brief Determine the name of a given membership/supplement package.
 *
 * @param id
 * @return std::string
 */
std::string packageNameLookup(std::string id)
{
    // We will check and return package name details.
    return packageDetailLookup("name", id);
}

/**
 * @brief Get the details the user requested.
 *
 * @param choice
 */
void getDetails(int choice)
{
    // We can only allow an existing index to be passed
    // in as the choice. e.x. 0, 1, 2, 3 only.
    try
    {
        // If the choice value is within the given range (0, 1, 2, 3) then we
        // get the name of the file the details are stored in, and retrieve
        // the file from the relevant location. Once the file is read the
        // content is printed out to the screen.
        std::cout << (readFile(details(choice))) << std::endl; // This function is found in "utils.cpp"
    }
    catch (...)
    {
        // In the case no valid choice is found we print out the
        // instaructions on how to properly use the application.
        std::cout << (readFile(details(0))) << std::endl; // This function is found in "utils.cpp"
    }
}
