#include "../src/includes/invoice.h"
#include "gtest/gtest.h"
#include <cstdlib>
#include <string>

TEST(invoice_tests, currentDateTime)
{
    time_t now = time(0);
    std::string dateTime = ctime(&now);

    EXPECT_EQ(dateTime, currentDateTime());
}
