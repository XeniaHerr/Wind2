#include "Logger.h"
#include "WindowManagerModel.h"
#include "structs.h"
#include <X11/X.h>

#include <Client.h>
#include <cmath>
#include <optional>
#include <sys/types.h>
#include <vector>
#include <RuleBuilder.h>


constexpr bool EnableRules = true;

using namespace Wind;

Client::Client(Window win) : _window(win) {

    this->is_orphan = true;
    this->is_floating = false;
    this->is_visible = true; //Default
    this->_owner  = nullptr;


    //    if constexpr (EnableRules)
    //       this->attachRule();

}


Client::Client(const Client&& other) : name(std::move(other.name)), windowclass(std::move(other.windowclass)) {
    this->_window = other._window;
    this->oldDimension = other.oldDimension;
    this->currentDimension = other.currentDimension;
    this->oldPosition = other.oldPosition;
    this->is_floating = other.is_floating;
    this->is_fullscreen = other.is_fullscreen;
    this->is_visible = other.is_visible;
    this->is_orphan = other.is_orphan;
    this->type = other.type;
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

    if(isOrphan()) {
	this->is_orphan = false;
	Logger::GetInstance().Info("Client {} no longer an orphan", this->name);
    }


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

auto Client::setFloating() -> void {
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
    //When a new Rule is attached, then the rule should also be used
    //applyRule();
}

auto Client::setName(std::string name) -> void {
    this->name = name;
}

auto Client::setClass(std::string s) -> void {
    this->windowclass = s;
}

auto Client::setType(Windowtype t) -> void {
    this->type = t;
}

auto Client::getName() const -> decltype(name) {
    return name;
}

auto Client::getClass() const -> decltype(windowclass) {
    return windowclass;
}


auto Client::getType() const -> decltype(type) {
    return this->type;
}


auto Client::applyRule() -> void {

    auto& Log = Logger::GetInstance();

    targetDimension.width = std::max(targetDimension.width, rules.minSize.width);
    targetDimension.height = std::max(targetDimension.height, rules.minSize.height);

    Log.Info("setTargetDimensions to {},{}", targetDimension.width, targetDimension.height);

    if (rules.maxSize.has_value()) {
	targetDimension.width = std::min(targetDimension.width, rules.maxSize->width);
	targetDimension.height = std::min(targetDimension.height, rules.maxSize->height);
	Log.Info("setTargetDimensions again to {},{}", targetDimension.width, targetDimension.height);
    }


    if (rules.keepAspectratio) {
	Log.Info("Apply getAspectratio");

	if (targetDimension.height != currentDimension.height) {

	    double_t ratio = static_cast<double_t>(currentDimension.width) / currentDimension.height;

	    targetDimension.width = ratio * targetDimension.height;

	} else if (targetDimension.width != currentDimension.width) {

	    double_t ratio = static_cast<double_t>(currentDimension.height) / currentDimension.width;

	    targetDimension.height = ratio * currentDimension.width;
	}
    }
    //If we already have an owner, we do nothing here
    if (this->is_orphan) {
	Topic *t;
	if (rules.targetTopic != std::nullopt) {
	    Log.Info("Found a default topic");

	    t  = WindowManagerModel::getInstance().getTopic(rules.targetTopic.value());
	}
	else { 
	    Log.Info("No default Topic found, using current topic");
	    t = WindowManagerModel::getInstance().getFocusedMon()->getCurrent();
	}

	//this->_owner = t;

	Log.Info("Take ownership");
	if (t != nullptr)
	    t->takeOwnership(*this);
	else
	    Log.Error("Nullptr found");

	Log.Info("Ownership set successfully");

    } else {
	Log.Info("Client already owned: This is {}", !this->is_orphan ? "True" : "False");
    }

}


auto Client::setManaged(bool b) -> void {
    this->is_managed = b;
} 


auto Client::isManaged() const ->  bool {
    return this->is_managed;
}


auto Client::toggleFullscreen() -> void {

    if (this->is_fullscreen)
	this->is_fullscreen = false;
    else
	this->is_fullscreen = true;
}


auto Client::isFullscreen() const -> bool {
    return this->is_fullscreen;
}
