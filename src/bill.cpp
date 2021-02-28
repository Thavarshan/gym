/**
 * @file bill.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "includes/details.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Calculate total payable amount and deduct discounts if user is eligible.
 *
 * @param purchases
 * @return std::map<std::string, double>
 */
std::map<std::string, double> calculateBillAmount(std::vector<double> &amounts)
{
    std::map<std::string, double> charges;
    double total = 0.00;

    for (double &amount : amounts)
    {
        total += amount;
    }

    double discount = total > 5000.00 ? total * 0.05 : 0.00;
    double due = total - discount;

    charges.insert(std::make_pair("total", total));
    charges.insert(std::make_pair("discount", discount));
    charges.insert(std::make_pair("due", due));

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
    std::vector<double> amounts;
    std::map<std::string, double> details;

    for (auto const &[id, units] : choices)
    {
        amounts.push_back(packagePriceLookup(id) * (double)units);

        details.insert(std::make_pair(id, (double)units));
    }

    for (auto const &[charge, amount] : calculateBillAmount(amounts))
    {
        details.insert(std::make_pair(charge, amount));
    }

    return details;
}
