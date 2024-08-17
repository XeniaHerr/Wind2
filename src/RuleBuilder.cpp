#include "Rules.h"
#include "structs.h"
#include <RuleBuilder.h>
#include <cmath>


using namespace Wind;




RuleBuilder::RuleBuilder() : _object() {

    _object.content.minSize = Dimensions(0,0);
    _object.content.maxSize = Dimensions(1 << 31, 1 << 31);
    _object.content.neverFocus = false;
    _object.content.forceFloat = false;
    _object.content.keepAspectratio = false;
    _object.content.targetTopic = "";
    _object.windowname = "";
    _object.windowclass = "";
    _object.type = Windowtype::ANYTYPE;


}

auto RuleBuilder::finish() -> decltype(_object) {
    return _object;
}

auto RuleBuilder::setAlwaysFloat() -> decltype(*this) {


    _object.content.forceFloat = true;
    return *this;
}

auto RuleBuilder::setNonfocusable() -> decltype(*this) {
    _object.content.neverFocus = true;
    return *this;
}

auto RuleBuilder::setAspectratio(double_t d) -> decltype(*this) {

    if (d < 0.2)
        _object.content.relation = 0.2;
    else if (d > 0.8)
        _object.content.relation = 0.8;
    else
        _object.content.relation = d;

    return *this;
}


auto RuleBuilder::lockAspectratio() -> decltype(*this) {
    _object.content.keepAspectratio = true;

    return *this;
}

auto RuleBuilder::setWindowName(string name) -> decltype(*this) {
    _object.windowname = name;

    return *this;
}

auto RuleBuilder::setWindowClass(string wclass) -> decltype(*this) {
    _object.windowclass = wclass;
    return *this;
}

auto RuleBuilder::setMinimalDimensions(Dimensions m) -> decltype(*this) {
    _object.content.minSize = m;

    return *this;
}


auto RuleBuilder::setMaximalDimensions(Dimensions m) -> decltype(*this) {
    _object.content.maxSize = m;
    return *this;
}

auto RuleBuilder::setWindowType(Windowtype t) -> decltype(*this) {
    _object.type = t;

    return *this;
}

auto RuleBuilder::setDefaultTopic(string tname) -> decltype(*this) {

    _object.content.targetTopic = tname;
    return *this;
}
