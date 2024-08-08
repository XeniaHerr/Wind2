#include <Client.h>
#include <Topic.h>




using namespace Wind;

Topic::Topic(std::string name) : name(name),
    holder(nullptr), focusedclient(nullptr){ }


auto Topic::getHolder() const -> decltype(*holder)& {
    return *this->holder;
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

    clients.push_front(&c);
    c.setOwner(*this);

}


auto Topic::releaseOwnership(Client& c) ->  const Client& {

    clients.remove(&c);

    return c;

}



auto Topic::getName() const -> decltype(name) {
    return name;
}


auto Topic::setFocus(Client* c) -> void {
    if(containsClient(*c) || c == nullptr) //Alway allow nullptr to signal that no client is focused
        focusedclient = c;
}

auto Topic::setFocus(Client& c) -> void {
    setFocus(&c);
}

auto Topic::getFocused() const -> decltype(focusedclient) {
    return this->focusedclient;
}


