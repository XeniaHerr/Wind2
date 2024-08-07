#include <Client.h>
#include <Topic.h>




using namespace Wind;

Topic::Topic(std::string name) : name(name){



}


auto Topic::getHolder() const -> decltype(*holder) {
    return *this->holder;
}


auto Topic::setHolder(Monitor* mon ) -> void {
    this->holder = mon;
}

auto Topic::getClients() -> decltype(clients)& {
    return this->clients;
}

auto Topic::adopt(Client& c ) -> void {


    if (containsClient(c))
        return;

    clients.push_front(&c);
    c.setOwner(*this);

}


auto Topic::abandon(Client& c) -> const Client& {

    clients.remove(&c);
    return c;
}


auto Topic::getName() const -> decltype(name) {
    return name;
}


