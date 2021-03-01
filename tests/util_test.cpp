#include "../src/includes/utils.h"
#include "gtest/gtest.h"

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
