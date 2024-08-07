#include "structs.h"
#include <X11/X.h>

#include <Client.h>
#include <ios>
#include <sys/types.h>



using namespace Wind;

Client::Client(Window win) : _window(win) {

    this->is_orphan = true;

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
