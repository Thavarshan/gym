#include "details.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

/**
 * @brief Calculate total payable amount and deduct discounts if user is eligible.
 *
 * @param purchases
 */
std::map<std::string, float> calculateBillAmount(std::vector<float> &amounts)
{
    std::map<std::string, float> charges;
    float total, discount, due = 0.00;

    for (float &amount : amounts)
    {
        total += amount;
    }

    if (total > 5000.00)
    {
        discount = (total * 0.05);
    }

    due = total - discount;

    charges.insert(std::make_pair("Total", total));
    charges.insert(std::make_pair("Discount", discount));
    charges.insert(std::make_pair("Due", due));

    return charges;
}

/**
 * @brief
 *
 * @param choices
 * @return std::map<std::string, float>
 */
std::map<std::string, float> makePurchase(std::map<std::string, int> &choices)
{
    std::vector<float> amounts;
    std::map<std::string, float> details;

    for (auto const &[id, units] : choices)
    {
        amounts.push_back(packagePriceLookup(id) * (float)units);

        details.insert(std::make_pair(id, (float)units));
    }

    for (auto const &[charge, amount] : calculateBillAmount(amounts))
    {
        details.insert(std::make_pair(charge, amount));
    }

    for (auto const &[charge, amount] : details)
    {
        std::cout << charge << " and " << amount << std::endl;
    }

    return details;
}
