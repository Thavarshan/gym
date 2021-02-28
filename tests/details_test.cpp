#include "../src/includes/details.h"
#include "gtest/gtest.h"

TEST(details_test, details)
{
    EXPECT_EQ("details/instructions.txt", details(0));
    EXPECT_EQ("details/packages.txt", details(1));
    EXPECT_EQ("details/supplements.txt", details(2));
    EXPECT_EQ("details/about.txt", details(3));
}

TEST(details_test, isPackage)
{
    EXPECT_TRUE(isPackage("PKGDT001"));
    EXPECT_FALSE(isPackage("FAKEPACKAGEID"));
    EXPECT_TRUE(isPackage("N/A"));
}

TEST(details_test, packagePriceLookup)
{
    EXPECT_EQ(1500.00, packagePriceLookup("PKGDT001"));
    EXPECT_EQ(0.00, packagePriceLookup("N/A"));
}

TEST(details_test, packageNameLookup)
{
    EXPECT_EQ("DAY WORKOUT", packageNameLookup("PKGDT001"));
    EXPECT_EQ("No supplements", packageNameLookup("N/A"));
    EXPECT_NE("FAKE PACKAGE NAME", packageNameLookup("PKGDT002"));
}
