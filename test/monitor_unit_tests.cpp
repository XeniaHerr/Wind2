#include "structs.h"
#include <gtest/gtest.h>




#include <Monitor.h>


TEST(MonitorTest,PositionandDimensionKonstruktorandGetter) {

    const Wind::Dimensions d(100, 200);
    const Wind::Position p(20, 10);
    Wind::Monitor m(d,p);


    EXPECT_EQ(m.getPosition(), p);
    EXPECT_EQ(m.getDimensions(), d);

     
}
