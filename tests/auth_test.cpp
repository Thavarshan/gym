#include "../src/include/auth.h"
#include "../src/include/details.h"
#include "gtest/gtest.h"
#include <map>
#include <string>

TEST(AuthTest, AuthUserLookup)
{
    users.insert(std::make_pair("tjthavarshan@gmail.com", "iforgetmypasswords"));

    EXPECT_EQ("iforgetmypasswords", authUserLookup("tjthavarshan@gmail.com"));
}

TEST(AuthTest, AuthenticateUser)
{
    users.insert(std::make_pair("tjthavarshan@gmail.com", "iforgetmypasswords"));

    std::map<std::string, std::string> correctCredentials{
        {"email", "tjthavarshan@gmail.com"},
        {"password", "iforgetmypasswords"},
    };

    std::map<std::string, std::string> incorrectCredentials{
        {"email", "jane@example.com"},
        {"password", "iremembermypasswords"},
    };

    EXPECT_TRUE(authenticate(correctCredentials));
    EXPECT_FALSE(authenticate(incorrectCredentials));
}
