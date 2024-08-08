#include <gtest/gtest.h>
#include <Client.h>
#include <Topic.h>





class Client_Topic_Integration : public testing::Test {
    protected:


        Wind::Client c, c2;

        Wind::Topic t, t2;

        Client_Topic_Integration() : 
            c(100), 
            t("Default"),
            t2("Second"),
            c2(200) {}


};



TEST_F(Client_Topic_Integration, getandSetOwner) {

    c.setOwner(t);

    EXPECT_EQ(&c.getOwner(), &t);
}


TEST_F(Client_Topic_Integration, adoptClientBasic) {


    EXPECT_EQ(c.isOrphan(), true);

    t.takeOwnership(c);

    EXPECT_EQ(c.isOrphan(), false);

    EXPECT_EQ(&c.getOwner(), &t);


}


TEST_F(Client_Topic_Integration, adoptClientAlreadyOwned) {
    
    t.takeOwnership(c);

    EXPECT_EQ(&c.getOwner(), &t);

    t2.takeOwnership(c);

    EXPECT_EQ(&c.getOwner(),&t2);
}


TEST_F(Client_Topic_Integration, getClientsNotEmpty) {

    EXPECT_EQ(t.getClients().size(), 0);


    t.takeOwnership(c);

    EXPECT_EQ(t.getClients().size(), 1);
}


TEST_F(Client_Topic_Integration, getClientBehaviouronMultipleTopics) {

    t.takeOwnership(c);

    t2.takeOwnership(c2);


    EXPECT_EQ(&c.getOwner(), &t);
    EXPECT_EQ(&c2.getOwner(), &t2);



    EXPECT_EQ(t.getClients().size(), 1);


    t2.releaseOwnership(c2);
    t.takeOwnership(c2);

    EXPECT_EQ(t.getClients().size(), 2);

    EXPECT_EQ(t2.getClients().size(), 0);
}



TEST_F(Client_Topic_Integration, GetandSetFocused) {


    t.takeOwnership(c);
    t.setFocus(&c);


    EXPECT_EQ(t.getFocused(),&c);
}

TEST_F(Client_Topic_Integration, GetandSetFocusedonClientnotOwned) {


    t.setFocus(&c);


    EXPECT_NE(t.getFocused(),&c);
}
