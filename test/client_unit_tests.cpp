#include "structs.h"
#include <gtest/gtest.h>
#include <Client.h>


TEST(ClientTest, windowAttribute) {
    Wind::Client c(100);

    EXPECT_EQ(c.getWindow(), 100);
}


TEST(ClientTest, getandsetDimensions) {

    Wind::Client c(100);

    c.setCurrentDimensions(Wind::Dimensions(1920, 1080));


    EXPECT_EQ(c.getCurrentDimensions(), Wind::Dimensions(1920, 1080));

}


TEST(ClientTest, testsetDimensionsxy) {
    Wind::Client c(100);

    c.setCurrentDimensions(1920, 1080);

    EXPECT_EQ(c.getCurrentDimensions(),Wind::Dimensions(1920, 1080));
}


TEST(ClientTest, getDimensionsstoresOldDimensions) {

    Wind::Client c(100);
   const Wind::Dimensions first(1920,1080), second(100, 200);

    c.setCurrentDimensions(first);

    c.setCurrentDimensions(second);

    EXPECT_EQ(c.getOldDimensions(), first);





}


