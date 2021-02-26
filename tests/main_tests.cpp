#include "../src/includes/bill.h"
#include "../src/includes/details.h"
#include "../src/includes/invoice.h"
#include "../src/includes/utils.h"
#include <gtest/gtest.h>

TEST(util_tests, inRange)
{
    EXPECT_TRUE(inRange(45, 50, 47));
    EXPECT_FALSE(inRange(45, 50, 30));
}

TEST(bill_tests, makePurchase)
{
    std::map<std::string, int> choices{{"PKGDT001", 1}, {"ITMP001", 1}};
    std::map<std::string, float> details{
        {"PKGDT001", 1},
        {"ITMP001", 1},
        {"total", 5500.00},
        {"discount", 275.00},
        {"due", 5227.00},
    };

    EXPECT_EQ(details, makePurchase(choices));
}

TEST(details_tests, details)
{
    EXPECT_EQ("details/instructions.txt", details(0));
    EXPECT_EQ("details/packages.txt", details(1));
    EXPECT_EQ("details/supplements.txt", details(2));
    EXPECT_EQ("details/about.txt", details(3));
}

TEST(details_tests, isPackage)
{
    EXPECT_TRUE(isPackage("PKGDT001"));
    EXPECT_FALSE(isPackage("FAKEPACKAGEID"));
    EXPECT_TRUE(isPackage("N/A"));
}

TEST(details_tests, packagePriceLookup)
{
    EXPECT_EQ(1500.00, packagePriceLookup("PKGDT001"));
    EXPECT_EQ(0.00, packagePriceLookup("N/A"));
}

TEST(details_tests, packageNameLookup)
{
    EXPECT_EQ("DAY WORKOUT", packageNameLookup("PKGDT001"));
    EXPECT_EQ("No supplements", packageNameLookup("N/A"));
    EXPECT_NE("FAKE PACKAGE NAME", packageNameLookup("PKGDT002"));
}

TEST(invoice_tests, currentDateTime)
{
    time_t now = time(0);
    char *dateTime = ctime(&now);

    EXPECT_EQ(dateTime, currentDateTime());
}

/**
 * @brief Execute test program.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
