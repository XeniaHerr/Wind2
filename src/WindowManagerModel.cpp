
#include <WindowManagerModel.h>
#include <X11/X.h>
#include <system_error>


using namespace Wind;

auto WindowManagerModel::moveClienttoTopic(Window w, u_int topicnumber) -> void {

        if (!clients.contains(w))
            return;
        if (topics[topicnumber].get() == clients[w].get().getOwner())
            return;

        clients[w].get().getOwner().releaseOwnership(clients[w].get());
        topics[topicnumber].get().takeOwnership(clients[w].get());
    }



auto WindowManagerModel::moveClienttoTopic(Window w, Topic& t) -> void {


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


auto WindowManagerModel::getClientCount()  -> u_int {
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



