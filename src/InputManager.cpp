#include "Inputstructs.h"
#include <InputManager.h>
#include <utility>


using namespace Wind;

auto InputManager::addKey(Key k, Action a) -> void {

    if(!this->keys.contains(k))
        this->keys.insert(std::make_pair(k, a));
}


auto InputManager::handleKey(Key k) -> void {

    auto action = keys.find(k);


    if (action != keys.end())
        action->second.call();
}
