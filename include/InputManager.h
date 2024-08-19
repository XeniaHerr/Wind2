#include <Action.h>
#include <Inputstructs.h>



#include <map>
namespace Wind {

class InputManager {


    public:


        static InputManager& GetInstance() {
            static InputManager m;
            return m;
        }

        void addKey(Key k, Action&& a);

        void addButton(Button b, Action&& a);


        void  handleKey(Key k);

        void handleButton(Button b);


        InputManager(InputManager& other) = delete;
        InputManager(InputManager&& other) = delete;

        InputManager& operator=(InputManager& other) = delete;
        InputManager& operator=(InputManager&& other) = delete;

    private:

        std::map<Key, Action> keys;


        std::map<Button, Action> buttons;


        InputManager() {}
};
}
