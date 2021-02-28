#include "gtest/gtest.h"

/**
 * @brief Execute test program.
 *
 * @param argc
 * @param argv
 * @return int
 */
int submain(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
