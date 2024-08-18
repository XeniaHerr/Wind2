#include "Topic.h"
#include "WindowManagerModel.h"
#include "structs.h"
#include <Monitor.h>
#include <Client.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <sys/types.h>
#include <vector>
#include <Logger.h>

constexpr bool AutoArrange = false;
constexpr bool Gaps = true;

using namespace Wind;


Monitor::Monitor(u_int32_t x, u_int32_t y) : realDimensions(x,y) {

    usableDimensions.width = x;
    usableDimensions.height = y - barHeight; //DO some calculations first;

    is_active = false;


    //currentArranger = &m;
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



Monitor::Monitor(Monitor&& other)  {
    usableDimensions = other.usableDimensions;
    realDimensions = other.realDimensions;

    usablePosition = other.usablePosition;

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


auto Monitor::setArranger(AbstractArranger* a) -> void {
    //this->currentArranger = a;
}


auto Monitor::arrange() -> void {

    auto& Log = Logger::GetInstance();

    AbstractArranger& currentArranger = layouts.getArranger();
    //assert(this->currentArranger != nullptr);
    Log.Info("Inside {}", __func__);


    auto clients = this->current->getClients();

    std::vector<Client*> new_clients;


    Log.Info("Prepare to filter");


    auto new_end = std::copy_if(clients.begin(), clients.end(), std::back_inserter(new_clients), [&](Client* c) { return  c->isVisible() == true;});

    u_int size = new_clients.size();

    Log.Info("Arranging {} Clients with Arranger {}", size, currentArranger.name());
    

    for(u_int i = 0; i < size; i++ ) {
         Dimensions d = currentArranger.getDimensions(*this,i+1, size );
         Position p = currentArranger.getPosition(*this, i+1 , size);

         if constexpr(Gaps)
             adjustforGaps(d,p);
         new_clients[i]->setTargetPositions(p);
         new_clients[i]->setTargetDimensions(d);
         Log.Info("Finished with client {}, New values are : Dim({},{}), Pos({},{})",i, d.width, d.height, p.x, p.y);
    }




}


auto Monitor::nextLayout() -> void {
    layouts.next();

    if constexpr (AutoArrange)
    arrange();
}


auto Monitor::prevLayout() -> void {
    layouts.previous();

    if constexpr (AutoArrange)
        arrange();
}



auto Monitor::adjustforGaps(Dimensions& d, Position& p) -> void {


    u_int16_t gaps = WindowManagerModel::getInstance().getGap();


    d.width -= gaps;
    d.height -= gaps;

    p.x += gaps/2;
    p.y += gaps/2;


}

