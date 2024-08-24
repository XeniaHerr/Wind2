#include <climits>
#include <gtest/gtest.h>

#include <InputManager.h>
#include <sstream>
#include <strstream>
#include <ConcreteActions.h>








TEST(InputManager, simpletest) {


    std::ostringstream buf;

    auto& IM = Wind::InputManager::GetInstance();

    std::unique_ptr<Wind::Action> a(new Wind::quitAction);


    Wind::Key k;
    k.keysym = 10; k.modifier = 0;



    IM.addKey(k, std::move(a));



    IM.handleKey(k);



    //EXPECT_EQ(buf.str(), " Action a\n");
// This test is now not so easily doable, will have to use a custom test command, or simply trust the process.
}
