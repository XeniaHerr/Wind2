#include "Topic.h"
#include "structs.h"
#include <Monitor.h>
#include <sys/types.h>


using namespace Wind;


Monitor::Monitor(u_int32_t x, u_int32_t y) : realDimensions(x,y) {

    usableDimensions.width = x;
    usableDimensions.height = y - barHeight; //DO some calculations first;

    is_active = false;
}


Monitor::Monitor(Dimensions d, Position p) :  realDimensions(d), realPosition(p),
    usableDimensions(d), usablePosition(p){
        adjustforBarHeight();

    }

Monitor::Monitor(Dimensions d, Position p, u_int8_t barHeight) :  realDimensions(d), realPosition(p), barHeight(barHeight) {

    usableDimensions.height = realDimensions.height - barHeight;
    usableDimensions.width = realDimensions.width;

    usablePosition.x = realPosition.x;
    usablePosition.y = realPosition.y + barHeight;
}
auto Monitor::getDimensions() const -> decltype(usableDimensions) {

    return this->usableDimensions;
}

auto Monitor::getPosition() const -> decltype(usablePosition) {
    return this->usablePosition;
}


auto Monitor::getCurrent() -> decltype(current) {
    return current;
}

auto Monitor::setCurrent(Topic* topic) -> void {
    this->current = topic;
}


auto Monitor::toggleBar() -> void {

    if (barHeight != 0) {
        if (this->realPosition != this->usablePosition || this->realDimensions != this->usableDimensions) {
            this->usableDimensions = this->realDimensions;
            this->usablePosition = this->realPosition;
        } else {
            adjustforBarHeight();
        }
    }
}


auto Monitor::setBarHeight(u_int8_t h) -> void {
    this->barHeight = h;
}


auto Monitor::adjustforBarHeight() -> void {

    usableDimensions.height = realDimensions.height - barHeight;
    usableDimensions.width = realDimensions.width;

    usablePosition.x = realPosition.x;
    usablePosition.y = realPosition.y + barHeight;


}



