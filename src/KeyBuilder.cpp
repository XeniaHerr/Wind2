#include <InputBuilder.h>
#include <X11/X.h>


using namespace Wind;



auto KeyBuilder::finish() -> Key {
    return this->k;
}


auto KeyBuilder::setKeySym(KeySym sym) -> decltype(*this) {
    //TODO: Logic that keysym is valid on keyboard
    k.keysym = sym;

    return *this;
}


auto KeyBuilder::setShiftMask() -> decltype(*this) {

    k.modifier |= ShiftMask;

    return *this;
}


auto KeyBuilder::setControlMask() -> decltype(*this) {

    k.modifier |= ControlMask;

    return *this;
}


auto KeyBuilder::setModMask() -> decltype(*this) {

    k.modifier |= _internal_choosen_modifier;

    return *this;
}
