#include "Logger.h"
#include <Client.h>
#include <Topic.h>
#include <mutex>




using namespace Wind;

Topic::Topic(std::string name) : name(name),
    holder(nullptr), focusedclient(nullptr){ }

Topic::Topic(Topic&& other) : name(std::move(other.name)), holder(other.holder), focusedclient(other.focusedclient) {

}


//Topic::Topic(Topic& other) : name(other.name) {}

//This method was of type Monitor&, which is stupid, because not every topic is on a monitor. This may very likley be a nullptr
auto Topic::getHolder() const -> decltype(holder) {
    return this->holder;
}


auto Topic::setHolder(Monitor* mon ) -> void {
    this->holder = mon;
}

auto Topic::getClients() -> decltype(clients)& {
    return this->clients;
}

auto Topic::takeOwnership(Client& c ) -> void {


    if (containsClient(c))
        return;

    Logger::GetInstance().Info("Client elligble");
    clients.push_front(&c);
    c.setOwner(*this);

    Logger::GetInstance().Info("{} now has {} clients", this->name, clients.size());

}


auto Topic::releaseOwnership(Client& c) ->  const Client& {

    clients.remove(&c);

    return c;

}



auto Topic::getName() const -> decltype(name) {
    return name;
}


auto Topic::setFocus(Client* c) -> bool {

    if(containsClient(*c) || c == nullptr) { //Alway allow nullptr to signal that no client is focused
        focusedclient = c;
	Logger::GetInstance().Warn("The currently focused client on Model site is {}", c ? c->getWindow() : 0);

        if (c) {
            clients.remove(c);
            clients.push_front(c);
	    return true;
        }
    } else {
	Logger::GetInstance().Warn("The currently focused client on Model site is {} (nothing changed)", focusedclient ? focusedclient->getWindow() : 0);
    }
    return false;
}

auto Topic::setFocus(Client& c) -> bool {
    return setFocus(&c);
}

auto Topic::getFocused() const -> decltype(focusedclient) {
    return this->focusedclient;
}



auto Topic::getMasterfact() const -> decltype(master_fact) {
    return this->master_fact;
}


auto Topic::setMasterfact(float t) -> void {
    this->master_fact = t;
}

