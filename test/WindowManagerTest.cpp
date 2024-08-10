#include <gtest/gtest.h>

#include "Topic.h"
#include "structs.h"
#include <WindowManagerModel.h>



TEST(WindowManagerModel, WindowManagerModelSingleton) {

    Wind::WindowManagerModel& first = Wind::WindowManagerModel::getInstance();
    Wind::WindowManagerModel& second = Wind::WindowManagerModel::getInstance();


    EXPECT_EQ(&first, &second);



}



class WindowManagerModelTest : public testing::Test {

    protected:

        Wind::WindowManagerModel& WMM;

        Wind::Client c, c1, c2;

        Wind::Topic t, t1, t2;

        Wind::Monitor m, m2;

        WindowManagerModelTest() :

            c(1), c1(2), c2(2),
            t("First"), t1("Second"), t2("Third"),
            m(Wind::Dimensions(1920, 1080), Wind::Position(1920, 0), 10),
            m2(Wind::Dimensions(1920, 1080), Wind::Position(1920, 0), 10),
            WMM(Wind::WindowManagerModel::getInstance())
            {


            }

};



TEST_F(WindowManagerModelTest, testManageWindow) {


    EXPECT_EQ(WMM.getClientCount(), 0);


    WMM.manageWindow(100);

    EXPECT_EQ(WMM.getClientCount(), 1);




}


TEST_F(WindowManagerModelTest, testtakeOwnershipRecordedinWMM) {

    WMM.manageWindow(100);


    t.takeOwnership(*WMM.getClient(100));

    EXPECT_EQ(&WMM.getClient(100)->getOwner(), &t);


}

TEST_F(WindowManagerModelTest, testManageWindowAgain) {


    EXPECT_EQ(WMM.getClientCount(), 0);


    WMM.manageWindow(100);

    EXPECT_EQ(WMM.getClientCount(), 1);

    WMM.manageWindow(100);

    EXPECT_EQ(WMM.getClientCount(), 1);
}


TEST_F(WindowManagerModelTest, testGetClient) {

    WMM.manageWindow(100);

    Wind::Client * c = WMM.getClient(100);


    EXPECT_NE(c, nullptr);
}

TEST_F(WindowManagerModelTest, testGetClientOnClientNotExisting) {

    WMM.manageWindow(100);

    Wind::Client * c = WMM.getClient(200);


    EXPECT_EQ(c, nullptr);
}

TEST_F(WindowManagerModelTest, testmoveClientToTopic) {


    WMM.manageWindow(100);


    t.takeOwnership(*WMM.getClient(100));

    EXPECT_EQ(&WMM.getClient(100)->getOwner(), &t);

    WMM.moveClienttoTopic(100, t2);

    EXPECT_EQ(&WMM.getClient(100)->getOwner(), &t2);

}



TEST_F(WindowManagerModelTest, testregisterTopic) {


    EXPECT_EQ(WMM.getTopicCount(), 0U);


    std::vector<std::string> names = {"VectorFirst", "Vectorsecond"}; 

    WMM.registerTopics(names);


    EXPECT_EQ(WMM.getTopicCount(), 2U);

}
