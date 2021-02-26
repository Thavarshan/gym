#include "../src/includes/utils.h"
#include <gtest/gtest.h>

TEST(util_tests, demotest)
{
    EXPECT_TRUE(true);
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
