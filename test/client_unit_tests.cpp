#include "structs.h"
#include <gtest/gtest.h>
#include <Client.h>


TEST(ClientTest, windowAttribute) {
    Wind::Client c(100);

    EXPECT_EQ(c.getWindow(), 100);
}


TEST(ClientTest, getandsetDimensions) {

    Wind::Client c(100);

    c.setDimensions(Wind::Dimensions(1920, 1080));


    EXPECT_EQ(c.getCurrentDimensions(), Wind::Dimensions(1920, 1080));

}


TEST(ClientTest, testsetDimensionsxy) {
    Wind::Client c(100);

    c.setDimensions(1920, 1080);

    EXPECT_EQ(c.getCurrentDimensions(),Wind::Dimensions(1920, 1080));
}


TEST(ClientTest, getDimensionsstoresOldDimensions) {

    Wind::Client c(100);
   const Wind::Dimensions first(1920,1080), second(100, 200);

    c.setDimensions(first);

    c.setDimensions(second);

    EXPECT_EQ(c.getOldDimensions(), first);
}


TEST(ClientTest, getandSetPositionwithold) {

    Wind::Client c(100);

    const Wind::Position p1(0,0), p2(100, 200);


    c.setPosition(p1);

    EXPECT_EQ(c.getPosition(), p1);


    c.setPosition(p2);

    EXPECT_EQ(c.getOldPosition(), p1);
}


TEST(ClientTest, testFloating) {
    Wind::Client c(100);
    EXPECT_EQ(c.isFloating(), false);


    c.toggleFloating();

    EXPECT_EQ(c.isFloating(), true);

}


