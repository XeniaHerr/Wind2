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

    //m.setArranger(&mon);

    Wind::Dimensions d;


    EXPECT_EQ(d, c.getCurrentDimensions());


    m.arrange();

    std::cerr << m.getDimensions().width << "," << m.getDimensions().height << std::endl;

    EXPECT_EQ(m.getDimensions(), c.getTargetDimension());
    EXPECT_EQ(m.getDimensions(), c3.getTargetDimension());
    EXPECT_EQ(m.getPosition(), c.getTargetPosition());
    EXPECT_EQ(m.getPosition(), c3.getTargetPosition());


    EXPECT_EQ(d, c2.getTargetDimension());


    auto cc = t.getClients();

    EXPECT_EQ(cc.size(), 3);





}



TEST(Arrange, testMasterFact) {

    ::testing::GTEST_FLAG(output) = "all";

    Wind::Monitor m(Wind::Dimensions(1920, 1080), Wind::Position(0,0),0);


    Wind::Client c(100), c2(101), c3(102);

   c.setVisible(true); c2.setVisible(true); c3.setVisible(true);

    Wind::Topic t("test");
    t.takeOwnership(c3);
    t.takeOwnership(c2);
    t.takeOwnership(c);

    Wind::Monocle mon;

    t.setHolder(&m);

    m.setCurrent(&t);


    t.setMasterfact(0.5);

    //m.setArranger(&mon);

    Wind::Dimensions d;


    EXPECT_EQ(d, c.getCurrentDimensions());
    EXPECT_EQ(d, c.getTargetDimension());

    m.nextLayout();

    m.arrange();


    Wind::Dimensions tc(1920/2, 1080);
    Wind::Dimensions tc2(1920/2, 1080/2);


    Wind::Position pc1(0,0);
    Wind::Position pc2(1920/2,0);
    Wind::Position pc3(1920/2, 1080/2);
    EXPECT_EQ(tc, c.getTargetDimension());
    EXPECT_EQ(m.getPosition(), c.getTargetPosition());
    EXPECT_EQ(pc2, c2.getTargetPosition());
    EXPECT_EQ(tc2, c2.getTargetDimension());
    EXPECT_EQ(pc3, c3.getTargetPosition());
    EXPECT_EQ(tc2, c3.getTargetDimension());





}


TEST(Arrange, testMasterFactsingleClient) {

    ::testing::GTEST_FLAG(output) = "all";

    Wind::Monitor m(Wind::Dimensions(1920, 1080), Wind::Position(0,0),0);


    Wind::Client c(100), c2(101), c3(102);

   c.setVisible(false); c2.setVisible(false); c3.setVisible(true);

    Wind::Topic t("test");
    t.takeOwnership(c3);
    t.takeOwnership(c2);
    t.takeOwnership(c);

    Wind::Monocle mon;

    t.setHolder(&m);

    m.setCurrent(&t);


    t.setMasterfact(0.5);

    //m.setArranger(&mon);

    Wind::Dimensions d;


    EXPECT_EQ(d, c.getCurrentDimensions());
    EXPECT_EQ(d, c.getTargetDimension());

    m.nextLayout();

    m.arrange();


    Wind::Dimensions tc(1920, 1080);
    Wind::Dimensions tc2(1920/2, 1080/2);


    Wind::Position pc1(0,0);
    Wind::Position pc2(1920/2,0);
    Wind::Position pc3(1920/2, 1080/2);

    std::cerr << m.getDimensions().width << "," << m.getDimensions().height << std::endl;
    EXPECT_EQ(pc1, c3.getTargetPosition());
    EXPECT_EQ(tc, c3.getTargetDimension());




}




