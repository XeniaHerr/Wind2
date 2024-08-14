#include<gtest/gtest.h>

#include "Topic.h"



TEST(TopicTest, TopictestgetName) {

    Wind::Topic t("Test");


    EXPECT_EQ(t.getName(), "Test");
}


TEST(TopicTest, TopictestdefaultHolder) {

    Wind::Topic t("test");

    EXPECT_EQ(t.getHolder(), nullptr);
}
