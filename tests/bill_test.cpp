#include "../src/include/bill.h"
#include "../src/include/details.h"
#include "gtest/gtest.h"
#include <string>

TEST(BillTest, MakePurchaseWithDiscount)
{
    struct Package record1;
    struct Package record2;

    record1.name = "DAY WORKOUT";
    record1.price = "1500";

    record2.name = "BEAST AMINOLYTES";
    record2.price = "4000";

    registry.insert(std::make_pair("PKGDT001", record1));
    registry.insert(std::make_pair("ITMP001", record2));

    std::map<std::string, int> choices{{"PKGDT001", 1}, {"ITMP001", 1}};
    std::map<std::string, double> details{
        {"PKGDT001", 1},
        {"ITMP001", 1},
        {"total", 5500.00},
        {"discount", 275.00},
        {"due", 5225.00},
    };

    EXPECT_EQ(details, makePurchase(choices));
}

TEST(BillTest, makePurchaseWithoutDiscount)
{
    struct Package record1;
    struct Package record2;

    record1.name = "DAY WORKOUT";
    record1.price = "1500";

    record2.name = "No supplements";
    record2.price = "0";

    registry.insert(std::make_pair("PKGDT001", record1));
    registry.insert(std::make_pair("N/A", record2));

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
