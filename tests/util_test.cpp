#include "../src/include/utils.h"
#include "gtest/gtest.h"
#include <string>
#include <vector>

TEST(UtilTest, ElementIsInList)
{
    std::vector<std::string> list = {"apple", "oranges"};

    EXPECT_TRUE(inList("apple", list));
    EXPECT_FALSE(inList("grapes", list));
}

TEST(UtilTest, IntegerIsInRange)
{
    EXPECT_TRUE(inRange(45, 50, 47));
    EXPECT_FALSE(inRange(45, 50, 30));
}

TEST(UtilTest, GetCurrentDateTime)
{
    time_t now = time(0);
    std::string dateTime = ctime(&now);

    EXPECT_EQ(dateTime, currentDateTime());
}
