
#include <WindowManagerModel.h>
#include <X11/X.h>
#include <iostream>


using namespace Wind;

auto WindowManagerModel::moveClienttoTopic(Window w, u_int topicnumber) -> void {

        if (!clients.contains(w) || topicnumber > getTopicCount())
            return;
        std::cerr << "Cleared first guard\n";
        if (&topics[topicnumber].get() == &clients[w].get().getOwner())
            return;

        std::cerr << "Cleared second guard\n";
        if (!clients[w].get().isOrphan())
            clients[w].get().getOwner().releaseOwnership(clients[w].get());
        std::cerr << "Released by prevoius owner\n";
        topics[topicnumber].get().takeOwnership(clients[w].get());
        std::cerr << "Adopted by new owner\n";
    }


//This is just a dummy implementation until the management of Topics is finalized.
auto WindowManagerModel::moveClienttoTopic(Window w, Topic& t) -> void {

    if (!clients.contains(w))
        return;

    if (&t == &clients[w].get().getOwner())
        return;


    clients[w].get().getOwner().releaseOwnership(clients[w].get());

    t.takeOwnership(clients[w].get());


}



auto moveTopictoMonitor(Topic& topic, Monitor& monitor) -> void {


        if (&topic.getHolder() == &monitor) return;

        if (monitor.getCurrent() != nullptr) {

            Monitor* orig = &topic.getHolder();

            auto old = monitor.getCurrent();

            old->setHolder(orig);

            orig->setCurrent(old);

            

        }

            monitor.setCurrent(&topic);

            topic.setHolder(&monitor);
    }



auto WindowManagerModel::manageWindow(Window w) -> void {

    if (!clients.contains(w)) {clients.emplace(w, ClientHolder(Client(w)));
    }

}


auto WindowManagerModel::getClientCount() const  -> u_int {
    return clients.size();
}


auto WindowManagerModel::focusClient(Window w) -> void {

    auto client = clients.find(w);


    if (client == clients.end())
        return;

    Client& c = client->second.get();


    Topic& t = c.getOwner();

    Monitor* m = &t.getHolder();


    if (m != this->focusedmon)
        focusedmon = m;


    t.setFocus(c);
}

auto WindowManagerModel::getClient(Window w) const -> Client* {

    auto it = clients.find(w);

    return it == clients.end() ? nullptr : &it->second.get();
}

auto WindowManagerModel::registerTopics(std::vector<std::string> parameters) -> void {

    for (auto name : parameters) {
        topics.emplace_back(TopicHolder(std::move(Topic(name))));
    }

}


auto WindowManagerModel::getTopicCount() const -> decltype(topics.size()) {
    return topics.size();
}


auto WindowManagerModel::getTopic(u_int topicnumber) const -> Topic* {


    if (topicnumber > this->getTopicCount())
        return nullptr;

    else
        return &this->topics[topicnumber].get();


}
