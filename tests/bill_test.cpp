#include "../src/includes/bill.h"
#include "gtest/gtest.h"
#include <string>

TEST(BillTest, MakePurchaseWithDiscount)
{
    std::map<std::string, int> choices{{"PKGDT001", 1}, {"ITMP001", 1}};
    std::map<std::string, double> details{
        {"PKGDT002", 1},
        {"ITMP001", 1},
        {"total", 5500.00},
        {"discount", 275.00},
        {"due", 5225.00},
    };

    EXPECT_EQ(details, makePurchase(choices));
}

TEST(BillTest, makePurchaseWithoutDiscount)
{
    std::map<std::string, int> choices{{"PKGDT001", 1}, {"N/A", 0}};
    std::map<std::string, double> details{
        {"PKGDT001", 1},
        {"N/A", 0},
        {"total", 1500.00},
        {"discount", 0.00},
        {"due", 1500.00},
    };

    EXPECT_EQ(details, makePurchase(choices));
}
