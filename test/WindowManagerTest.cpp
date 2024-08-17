#include <gtest/gtest.h>

#include "Rules.h"
#include "Topic.h"
#include "structs.h"
#include <WindowManagerModel.h>
#include <tuple>
#include <RuleBuilder.h>



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

    std::vector<std::string> names = {"VectorFirst", "Vectorsecond"}; 

    WMM.registerTopics(names);

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



    EXPECT_EQ(WMM.getTopicCount(), 2U);


}


TEST_F(WindowManagerModelTest, testmoveClientToTopicwithrealFunction) {
    
    ::testing::GTEST_FLAG(output) = "all";

    WMM.manageWindow(100);


    auto c = WMM.getClient(100);


    c->setOwner(t);


    WMM.moveClienttoTopic(100, 0);


    EXPECT_EQ(&WMM.getClient(100)->getOwner(), WMM.getTopic(0));



}



TEST_F(WindowManagerModelTest, testRegisterMonitors) {

    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10)};

    EXPECT_EQ(WMM.getMonitorCount(), 0);

    WMM.registerMonitors(monargs);


    EXPECT_EQ(WMM.getMonitorCount(), 2);

}

TEST_F(WindowManagerModelTest, testfocusClient) {

    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10)};

    EXPECT_EQ(WMM.getFocusedMon(), nullptr);
    

    WMM.manageWindow(100);

    WMM.getClient(100)->setOwner(*WMM.getTopic(0));


    WMM.registerMonitors(monargs);

    auto t = WMM.getTopic(0);

    t->takeOwnership(*WMM.getClient(100));

    auto m = WMM.getMonitor(0);

    m->setCurrent(t);

    t->setHolder(m);



    //This will not work until i can register Monitors
    WMM.focusClient(100);


    EXPECT_EQ(WMM.getClient(100)->getOwner().getFocused(), WMM.getClient(100));

    EXPECT_EQ(WMM.getFocusedMon(), m);




}

TEST_F(WindowManagerModelTest, testSwitchFocusClient) {

    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10)};

    EXPECT_EQ(WMM.getFocusedMon(), nullptr);
    

    WMM.manageWindow(100);

    WMM.getClient(100)->setOwner(*WMM.getTopic(0));


    WMM.registerMonitors(monargs);

    auto t = WMM.getTopic(0);

    t->takeOwnership(*WMM.getClient(100));

    auto m = WMM.getMonitor(0);

    m->setCurrent(t);

    t->setHolder(m);



    WMM.manageWindow(200);


     auto c1 = WMM.getClient(200);


     auto t2 = WMM.getTopic(1);

     auto m2 = WMM.getMonitor(1);

     t2->takeOwnership(*c1);

     m2->setCurrent(t2);

     t2->setHolder(m2);



    //This will not work until i can register Monitors
    WMM.focusClient(100);


    EXPECT_EQ(WMM.getClient(100)->getOwner().getFocused(), WMM.getClient(100));

    EXPECT_EQ(WMM.getFocusedMon(), m);



    WMM.focusClient(200);


    EXPECT_EQ(WMM.getClient(200)->getOwner().getFocused(), WMM.getClient(200));

    EXPECT_EQ(WMM.getFocusedMon(), m2);




}



TEST_F(WindowManagerModelTest, testunmanageClient) {

    WMM.manageWindow(100);

    EXPECT_NE(WMM.getClient(100), nullptr);


    WMM.unmanageWindow(100);
    EXPECT_EQ(WMM.getClient(100), nullptr);
}




TEST_F(WindowManagerModelTest, testdefaultRuleAttatchment) {

    WMM.manageWindow(100);


    auto r = WMM.getClient(100)->getRule();


    EXPECT_EQ(r.forceFloat, false);
}


TEST_F(WindowManagerModelTest, testCustomRuleAttatchment) {

    //I cant test this functionality right now, because i have no system to set names or Windowtypes for the client.
}


TEST_F(WindowManagerModelTest, testTopicMove) {

    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10)};

    WMM.registerMonitors(monargs);


    auto m = WMM.getMonitor(0);

    auto t = WMM.getTopic(0);

    m->setCurrent(WMM.getTopic(1));


    WMM.getTopic(1)->setHolder(m);






    WMM.moveTopictoMonitor(*WMM.getTopic(0), *WMM.getMonitor(0));


    EXPECT_EQ(t->getHolder(), m);


    EXPECT_NE(WMM.getTopic(1)->getHolder(),m );

}



TEST_F(WindowManagerModelTest, testTopicMoveTopicnotcurrentlyShown) {

    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10)};

    WMM.registerMonitors(monargs);


    auto m = WMM.getMonitor(0);

    auto t = WMM.getTopic(0);
    m->setCurrent(WMM.getTopic(1));

    WMM.getTopic(1)->setHolder(m);


    WMM.moveTopictoMonitor(*WMM.getTopic(0), *WMM.getMonitor(0));


    EXPECT_EQ(t->getHolder(), m);

    EXPECT_EQ(WMM.getTopic(1)->getHolder(), nullptr);

    EXPECT_EQ(m->getCurrent(), t);

}


TEST_F(WindowManagerModelTest, testTopicMoveTopicCurrentlyShown) {

    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10)};

    WMM.registerMonitors(monargs);


    auto m = WMM.getMonitor(0);

    auto t = WMM.getTopic(0);
    m->setCurrent(WMM.getTopic(1));

    WMM.getTopic(1)->setHolder(m);


    WMM.getMonitor(1)->setCurrent(t);

    t->setHolder(WMM.getMonitor(1));

    WMM.moveTopictoMonitor(*WMM.getTopic(0), *WMM.getMonitor(0));


    EXPECT_EQ(t->getHolder(), m);

    EXPECT_EQ(WMM.getTopic(1)->getHolder(), WMM.getMonitor(1));

    EXPECT_EQ(m->getCurrent(), t);

    EXPECT_EQ(WMM.getMonitor(1)->getCurrent(), WMM.getTopic(1));

}
