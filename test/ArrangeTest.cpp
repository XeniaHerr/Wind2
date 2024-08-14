#include <gtest/gtest.h>
#include <Monitor.h>
#include <Client.h>
#include <Topic.h>
#include <ConcreteArranger.h>


TEST(Arrange, testSimpleArrange) {

    ::testing::GTEST_FLAG(output) = "all";

    Wind::Monitor m(Wind::Dimensions(1920, 1080), Wind::Position(0,0),10);


    Wind::Client c(100), c2(101), c3(101);

   c.setVisible(true); c2.setVisible(true); c3.setVisible(true);

    Wind::Topic t("test");
    t.takeOwnership(c);
    t.takeOwnership(c2);
    t.takeOwnership(c3);

    Wind::Monocle mon;

    t.setHolder(&m);

    m.setCurrent(&t);

    m.setArranger(&mon);

    Wind::Dimensions d;


    EXPECT_EQ(d, c.getCurrentDimensions());
    EXPECT_EQ(d, c.getTargetDimension());


    m.arrange();

    std::cerr << m.getDimensions().width << "," << m.getDimensions().height << std::endl;

    EXPECT_EQ(m.getDimensions(), c.getTargetDimension());
    EXPECT_EQ(m.getDimensions(), c.getTargetDimension());
    EXPECT_EQ(m.getPosition(), c2.getTargetPosition());
    EXPECT_EQ(m.getDimensions(), c2.getTargetDimension());
    EXPECT_EQ(m.getPosition(), c3.getTargetPosition());
    EXPECT_EQ(m.getPosition(), c3.getTargetPosition());



    
}



TEST(Arrange, testArrangeFilter) {



    ::testing::GTEST_FLAG(output) = "all";

    Wind::Monitor m(Wind::Dimensions(1920, 1080), Wind::Position(0,0),10);


    Wind::Client c(100), c2(101), c3(101);

   c.setVisible(true); c2.setVisible(false); c3.setVisible(true);

    Wind::Topic t("test");
    t.takeOwnership(c);
    t.takeOwnership(c2);
    t.takeOwnership(c3);

    Wind::Monocle mon;

    t.setHolder(&m);

    m.setCurrent(&t);

    m.setArranger(&mon);

    Wind::Dimensions d;


    EXPECT_EQ(d, c.getCurrentDimensions());


    m.arrange();

    std::cerr << m.getDimensions().width << "," << m.getDimensions().height << std::endl;

    EXPECT_EQ(m.getDimensions(), c.getTargetDimension());
    EXPECT_EQ(m.getDimensions(), c.getTargetDimension());
    EXPECT_EQ(m.getPosition(), c3.getTargetPosition());
    EXPECT_EQ(m.getPosition(), c3.getTargetPosition());


    EXPECT_EQ(d, c2.getTargetDimension());


    auto cc = t.getClients();

    EXPECT_EQ(cc.size(), 3);





}
