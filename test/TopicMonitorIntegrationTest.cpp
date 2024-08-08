#include "structs.h"
#include <gtest/gtest.h>

#include <Monitor.h>
#include <Topic.h>
#include <memory>






class TopicMon : public testing::Test {

    protected:
        Wind::Monitor w, w2;

        Wind::Topic t, t2;




        TopicMon() :
            w(Wind::Dimensions(1920, 1080), Wind::Position(0,0),0),
            w2(Wind::Dimensions(1920, 1080), Wind::Position(1920,0),0),
            t("First"), t2("Second")
    {}

};




TEST_F(TopicMon, getandsetHolder) {

    t.setHolder(&w);


    EXPECT_EQ(&t.getHolder(), &w);
}


TEST_F(TopicMon, getandsetCurrent) {

    t.setHolder(&w);

    auto pt = std::make_shared<Wind::Topic>(t);

    w.setCurrent(pt);

    auto res = w.getCurrent();
    EXPECT_EQ((res.get()), (pt.get()));


}
