#include <Action.h>
#include <Inputstructs.h>



#include <algorithm>
#include <map>
#include <memory>
namespace Wind {

class InputManager {


    public:


        static InputManager& GetInstance() {
            static InputManager m;
            return m;
        }

        void addKey(Key k, std::unique_ptr<Action> a);

        void addButton(Button b, std::unique_ptr<Action> a);


        void  handleKey(Key k);

        void handleButton(Button b);


        InputManager(InputManager& other) = delete;
        InputManager(InputManager&& other) = delete;

        InputManager& operator=(InputManager& other) = delete;
        InputManager& operator=(InputManager&& other) = delete;

	std::vector<Key> getKeys();

    private:

        std::map<Key, std::unique_ptr<Action>> keys;


        std::map<Button, std::unique_ptr<Action>> buttons;


        InputManager() {}
};
}
