/**
 * @file bill.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.4.2
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/details.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Calculate total payable amount and deduct discounts if user is eligible.
 *
 * @param amounts
 * @return std::map<std::string, double>
 */
std::map<std::string, double> calculateBillAmount(std::vector<double> &amounts)
{
    // We define a map to store the charge title and amount like total, discount and due.
    std::map<std::string, double> charges;
    double total = 0.00;

    // We break down the vector that contains the prices of each packages purchased by the user.
    for (double &amount : amounts)
    {
        // And add each amount to sum up as the total amount.
        total += amount;
    }

    // We now check if the user is eligible for a discount by
    // determining if the total is greater than Rs. 5000
    double discount = total > 5000.00 ? total * 0.05 : 0.00;
    double due = total - discount; // the due total amount we set according to the discount value.

    // We now set each charge amount along with it's relevant titles.
    charges.insert(std::make_pair("total", total));
    charges.insert(std::make_pair("discount", discount));
    charges.insert(std::make_pair("due", due));

    // Finally we return the map with all the content.
    return charges;
}

/**
 * @brief Calculate bill amount for user choices invoice.
 *
 * @param choices
 * @return std::map<std::string, double>
 */
std::map<std::string, double> makePurchase(std::map<std::string, int> &choices)
{
    // The amounts vector will contain all the prices multiplied
    // by the number of each package purchased by the user.
    std::vector<double> amounts;
    // The details will have the final content that will be passed to the invoice generator module.
    std::map<std::string, double> details;

    // We first break down the user's purchase choices and calculate total price of each purchase.
    for (auto const &[id, units] : choices)
    {
        // Each package is recieved as a collection of it ID and units purchased
        // e.x. { "PKGDT001", 10 }. So, we lookup the price of each package
        // using its ID and multiply it by the number of units purchased.
        amounts.push_back(packagePriceLookup(id) * (double)units); // This function is found in "details.cpp".

        // We also have to pass these details to the invoice generator.
        details.insert(std::make_pair(id, (double)units));
    }

    // Now we calculate the final payable bill amounts.
    for (auto const &[charge, amount] : calculateBillAmount(amounts))
    {
        // We add those information to the details collection too.
        details.insert(std::make_pair(charge, amount));
    }

    // Finally we return all the information as a map.
    return details;
}
