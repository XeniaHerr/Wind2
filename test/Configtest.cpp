#include <gtest/gtest.h>
#include "structs.h"
#include <X11/X.h>
#include <ConfigReader.h>
#include <Logger.h>
#include <InputManager.h>






TEST(ConfigReader, firstReadTest) {


    Wind::ConfigReader& R = Wind::ConfigReader::getInstance();


    R.read("/home/xenia/Projects/Wind2/test/Wind.yaml");


    EXPECT_EQ(R._configs.borderwidth, 10);



}


TEST(ConfigReader, testBadPath) {

    auto& r = Wind::ConfigReader::getInstance();


    EXPECT_EQ(r.read("/home/xenia/Projects/Wind2/test/config.yaml"), false);
}


TEST(ConfigReader, testreadTopicNames) {

    auto& r = Wind::ConfigReader::getInstance();

    r.read("/home/xenia/Projects/Wind2/test/Wind.yaml");


    EXPECT_EQ(r._configs.topicnames[0], "First");
    EXPECT_EQ(r._configs.topicnames[1], "Second");
    EXPECT_EQ(r._configs.topicnames[2], "Third");
}

TEST(ConfigReader, testDimensionreader) {

    Wind::ConfigReader::getInstance().read("/home/xenia/Projects/Wind2/test/Wind.yaml");

    EXPECT_EQ(Wind::ConfigReader::getInstance()._configs.testdim, Wind::Dimensions(10,20));
    
    }


TEST(ConfigReader, testTypeReader) {

    Wind::ConfigReader::getInstance().read("/home/xenia/Projects/Wind2/test/Wind.yaml");


    EXPECT_EQ(Wind::ConfigReader::getInstance()._configs.testtype, Wind::Windowtype::DOCK);

}


TEST(ConfigReader, testRuleReader) {

    auto & R = Wind::ConfigReader::getInstance();
    R.read("/home/xenia/Projects/Wind2/test/Wind.yaml");


    EXPECT_EQ(R._configs.rules.size(), 2);


    EXPECT_EQ(R._configs.rules[0].content.targetTopic, "Second");


    EXPECT_EQ(R._configs.rules[1].content.minSize, Wind::Dimensions(90,100));

    EXPECT_EQ(R._configs.rules[0].content.forceFloat, true);

}



TEST(ConfigReader, testKeyReader) {



    Logger::GetInstance().setOutputfile("ConfigLog");

    auto & R = Wind::ConfigReader::getInstance();
    R.read("/home/xenia/Projects/Wind2/test/Wind.yaml");



    auto& IM = Wind::InputManager::GetInstance();


    Wind::Key k;
    k.keysym = 0;
    k.modifier = ShiftMask|Mod4Mask;

    EXPECT_EQ(R.localkeys.contains(k), true);

    for (auto a : R.localkeys) {

        auto l = a.first;
    Logger::GetInstance().Info("Read the following key {}, {}", l.keysym, l.modifier );
    }





}
