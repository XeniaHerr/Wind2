#include "structs.h"
#include <X11/X.h>

#include <Client.h>
#include <sys/types.h>



using namespace Wind;

Client::Client(Window win) : _window(win) {

    this->is_orphan = true;

}
auto Client::setCurrentDimensions(Dimensions dimensions) -> void {
    this->oldDimension = this->currentDimension;
    this->currentDimension = dimensions;
    
}

auto Client::setCurrentDimensions(u_int32_t x, u_int32_t y) -> void {
    this->oldDimension = this->currentDimension;
    this->currentDimension = Dimensions(x,y);
    
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
