#include "structs.h"
#include <Monitor.h>
#include <sys/types.h>


using namespace Wind;


Monitor::Monitor(u_int32_t x, u_int32_t y) : realDimensions(x,y) {

                usableDimensions.width = x;
                usableDimensions.height = y - barHeight; //DO some calculations first;
                
                is_active = false;
}


Monitor::Monitor(Dimensions d, Position p) :  realDimensions(d), realPosition(p) {

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

auto Monitor::setCurrent(std::weak_ptr<Topic> topic) -> void {
    this->current = topic;
}


