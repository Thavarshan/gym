#include "gtest/gtest.h"
#include "../src/include/db.h"

/**
 * @brief Execute test program.
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    loadData();

    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
