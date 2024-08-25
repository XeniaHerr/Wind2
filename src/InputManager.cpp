#include "Inputstructs.h"
#include "Logger.h"
#include <InputManager.h>
#include <X11/Xlib.h>
#include <cassert>
#include <memory>
#include <utility>


using namespace Wind;



auto InputManager::addKey(Key k, std::unique_ptr<Action> a) -> void {

    if(!this->keys.contains(k))
        this->keys.insert(std::make_pair(k, std::move(a)));

    assert(keys[k] != nullptr);
}


auto InputManager::handleKey(Key k) -> void {

    auto action = keys.find(k);

    Logger::GetInstance().Info("Handling Event for Key with sym = {} and modifiers = {}", k.keysym, k.modifier);


    if (action != keys.end()) {
	if(action->second == nullptr)
	    Logger::GetInstance().Error("Action is nullptr");
	else
        action->second->execute();
    }
}


auto InputManager::getKeys() -> std::vector<Key> {

    std::vector<Key> ret;

    for (auto& k : this->keys)
	ret.push_back(k.first);

    return ret;
}
