#include "../src/include/auth.h"
#include "gtest/gtest.h"
#include <map>
#include <string>

TEST(AuthTest, AuthenticateUser)
{
    std::map<std::string, std::string> correctCredentials{
        {"tjthavarshan@gmail.com", "iforgetmypasswords"},
    };

    std::map<std::string, std::string> incorrectCredentials{
        {"thavarshan@gmail.com", "iremembermypasswords"},
    };

    EXPECT_TRUE(authenticate(correctCredentials));
    EXPECT_TRUE(authenticate(incorrectCredentials));
}
