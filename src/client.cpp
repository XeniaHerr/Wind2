#include "WindowManagerModel.h"
#include "structs.h"
#include <X11/X.h>

#include <Client.h>
#include <sys/types.h>
#include <vector>
#include <RuleBuilder.h>


constexpr bool EnableRules = true;

using namespace Wind;

Client::Client(Window win) : _window(win) {

    this->is_orphan = true;
    this->is_floating = false;


    if constexpr (EnableRules)
        this->attachRule();

}


Client::Client(const Client&& other) {
    this->_window = other._window;
    this->oldDimension = other.oldDimension;
    this->currentDimension = other.currentDimension;
    this->oldPosition = other.oldPosition;
    this->is_floating = other.is_floating;
    this->is_fullscreen = other.is_fullscreen;
    this->is_visible = other.is_visible;
    this->is_orphan = other.is_orphan;
}
auto Client::setDimensions(Dimensions dimensions) -> void {
    this->oldDimension = this->currentDimension;
    this->currentDimension = dimensions;
    
}

auto Client::setDimensions(u_int32_t width, u_int32_t height) -> void {
    this->oldDimension = this->currentDimension;
    this->currentDimension = Dimensions(width,height);
    
}

auto Client::getCurrentDimensions() const -> decltype(currentDimension) {
    return currentDimension;
}


auto Client::getOldDimensions() const -> decltype(oldDimension) {
    return this->oldDimension;
}

auto Client::getOwner() const -> decltype(*_owner) { 
    return *this->_owner; 
}



auto Client::getWindow() const -> decltype(_window) { 
    return _window;
}

auto Client::setOwner(Topic& t) -> void {
    this->_owner = &t;

    if(isOrphan()) 
        this->is_orphan = false;


}


auto Client::isOrphan() const -> bool {
    return this->is_orphan;
}


auto Client::getPosition() const -> decltype(currentPosition) {
    return this->currentPosition;
}


auto Client::setPosition(Position pos) -> void {
    this->oldPosition = currentPosition;
    this->currentPosition = pos;
}


auto Client::getOldPosition() const -> decltype(oldPosition) {
    return this->oldPosition;
}


auto Client::isFloating() const -> bool {
    return this->is_floating;
}


auto Client::isVisible() const -> bool {
    return this->is_visible;
}




auto Client::toggleFloating() -> void {
    if (isFloating()) {
        this->is_floating = false;
    } else
        this->is_floating = true;
}



auto Client::setTargetPositions(Position p) -> void {
    this->targetPosition = p;
}


auto Client::setTargetDimensions(Dimensions d) -> void {

    this->targetDimension = d;
}


auto Client::getTargetDimension() const -> decltype(targetDimension) {

    return this->targetDimension;
}


auto Client::getTargetPosition() const -> decltype(targetPosition) {
    return this->targetPosition;
}


auto Client::setVisible(bool t) -> void {
    this->is_visible = t;
}
auto Client::setRule(Rule::RuleContent r) -> void {

    this->rules = r;
}


auto Client::getRule() const -> const decltype(rules) {

    return this->rules;

}


auto Client::attachRule() -> void {


    auto& all_rules = WindowManagerModel::getInstance().getRules();

    int level = 0, value = 0;
    Rule* r = all_rules[0].getPointer();


    for (auto & a : all_rules)
        if ((value = a.get().isApplicable("Name", "Class", Windowtype::ANYTYPE)) > level) {
            level = value;
            r = a.getPointer();
    }

    this->setRule(r->content);
}
