#include <gtest/gtest.h>
#include <WindowManagerModel.h>



TEST(WindowManagerModel, WindowManagerModelSingleton) {

    Wind::WindowManagerModel& first = Wind::WindowManagerModel::getInstance();
    Wind::WindowManagerModel& second = Wind::WindowManagerModel::getInstance();


    EXPECT_EQ(&first, &second);



}

