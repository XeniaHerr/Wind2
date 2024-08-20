#include <climits>
#include <gtest/gtest.h>

#include <InputManager.h>
#include <sstream>
#include <strstream>








TEST(InputManager, simpletest) {


    std::ostringstream buf;

    auto& IM = Wind::InputManager::GetInstance();


    Wind::Action a([&]( auto r){
            buf <<" Action a\n";
            }, 0, false);


    Wind::Key k;
    k.keysym = 10; k.modifier = 0;



    IM.addKey(k, a);



    IM.handleKey(k);



    EXPECT_EQ(buf.str(), " Action a\n");

}
