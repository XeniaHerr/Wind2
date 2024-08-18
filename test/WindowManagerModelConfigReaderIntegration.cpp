#include <gtest/gtest.h>

#include <WindowManagerModel.h>
#include <ConfigReader.h>




TEST(ApplyConfigTest, FirstTest) {


    auto& R = Wind::ConfigReader::getInstance();

    R.read("/home/xenia/Projects/Wind2/test/Wind.yaml");


    std::vector<std::tuple<Wind::Dimensions, Wind::Position, u_int>> monargs = { std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(0,0),10 ), 
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(1920,0), 10),
                                                                                std::make_tuple(Wind::Dimensions(1920,1080), Wind::Position(3840,0), 10)};

    auto& WMM = Wind::WindowManagerModel::getInstance();

    WMM.registerMonitors(monargs);


    WMM.loadConfig();


    EXPECT_EQ(WMM.getTopic(0)->getHolder(), WMM.getMonitor(0));
    EXPECT_EQ(WMM.getTopic(1)->getHolder(), WMM.getMonitor(1));
    EXPECT_EQ(WMM.getTopic(2)->getHolder(), WMM.getMonitor(2));

}
