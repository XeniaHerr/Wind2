#include <cmath>
#include <gtest/gtest.h>

#include "Rules.h"
#include "structs.h"
#include <RuleBuilder.h>
#include <unistd.h>




TEST(RuleTest, DefaultRule) {

    Wind::RuleBuilder g;

    Wind::Rule r = g.finish();


    EXPECT_EQ(r.content.minSize, Wind::Dimensions(0,0));


    EXPECT_EQ(r.content.forceFloat, false);
    EXPECT_EQ(r.content.targetTopic, "");
}


TEST(RuleTest, CustomRule) {

    Wind::Dimensions d (100, 200);

    double_t aspect = 0.5;

    Wind::Rule r = Wind::RuleBuilder().setMinimalDimensions(d)
                                      .setAspectratio(aspect)
                                      .setNonfocusable()
                                      .finish();

    EXPECT_EQ(r.content.minSize, d);
    EXPECT_EQ(r.content.relation, aspect);
    EXPECT_EQ(r.content.neverFocus, true);
}


TEST(RuleTest, AspectratioGuards) {

    Wind::Rule r = Wind::RuleBuilder().setAspectratio(0.1).finish();

    EXPECT_EQ(r.content.relation, 0.2);


    r = Wind::RuleBuilder().setAspectratio(0.81).finish();

    EXPECT_EQ(r.content.relation, 0.8);
}


TEST(RuleTest, ApplicableTest) {

    Wind::Rule r = Wind::RuleBuilder().finish();


    u_int first = r.isApplicable("", "", Windowtype::ANYTYPE);
    u_int second = r.isApplicable("", "", Windowtype::DOCK);
    u_int third = r.isApplicable("", "wrong", Windowtype::WIDGET);
    u_int fourth = r.isApplicable("wrong2", "wrong", Windowtype::SPLASH);
    u_int fith = r.isApplicable("wrong", "", Windowtype::BAR);


    EXPECT_EQ(first, 3U);

    EXPECT_EQ(second, 3U);


    EXPECT_EQ(third, 3U);

    EXPECT_EQ(fourth, 0U);


    EXPECT_EQ(fith, 2U);


}
