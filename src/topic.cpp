#include "Logger.h"
#include <Client.h>
#include <Topic.h>




using namespace Wind;

Topic::Topic(std::string name) : name(name),
    holder(nullptr), focusedclient(nullptr), fullscreen(nullptr), master_fact(0.5){
	Logger::GetInstance().Info("Creating topic");}

Topic::Topic(Topic&& other) : name(std::move(other.name)), holder(other.holder), focusedclient(other.focusedclient), master_fact(other.master_fact), clients(other.clients), fullscreen(other.fullscreen) {
Logger::GetInstance().Info("Moving topic");
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
    stack.push_front(&c);
    c.setOwner(*this);

    Logger::GetInstance().Info("{} now has {} clients", this->name, clients.size());

}


auto Topic::releaseOwnership(Client& c) ->  const Client& {
    Logger::GetInstance().Info("Released Ownership of {}, now holding {} clients", c.getWindow(), clients.size());

    clients.remove(&c);

    stack.remove(&c);
    return c;

}

auto Topic::getStack() -> decltype(stack)& {
    return stack;
}


auto Topic::getName() const -> std::string {
    return name;
}


auto Topic::setFocus(Client* c) -> bool {

    if(containsClient(*c) || c == nullptr) { //Alway allow nullptr to signal that no client is focused
        focusedclient = c;
	Logger::GetInstance().Warn("The currently focused client on Model site is {}", c ? c->getWindow() : 0);

        if (c) {
            stack.remove(c);
            stack.push_front(c);
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

