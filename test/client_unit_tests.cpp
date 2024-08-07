#include <gtest/gtest.h>
#include <Client.h>


TEST(ClientTest, windowAttribute) {
    Wind::Client c(100);

    EXPECT_EQ(c.getWindow(), 100);

}


