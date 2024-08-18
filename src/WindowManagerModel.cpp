#include "structs.h"
#include <WindowManagerModel.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <iostream>
#include <sys/types.h>
#include <utility>
#include <Rules.h>
#include <RuleBuilder.h>
#include <ConfigReader.h>


using namespace Wind;


 WindowManagerModel::WindowManagerModel() {


     this->focusedmon = nullptr;

     this->windowgaps = 0;

    //Create Default rule that applies to all clients
    
    rules.emplace_back(RuleHolder(RuleBuilder().finish()));

}

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



auto WindowManagerModel::moveTopictoMonitor(Topic& topic, Monitor& monitor) -> void {


        if (topic.getHolder() == &monitor) return;

        if (monitor.getCurrent() != nullptr) {

            Monitor* orig = topic.getHolder();

            auto old = monitor.getCurrent();

            old->setHolder(orig);


            if(orig != nullptr)
                orig->setCurrent(old);

            

        }

            monitor.setCurrent(&topic);

            topic.setHolder(&monitor);
    }



auto WindowManagerModel::manageWindow(Window w) -> void {

    if (!clients.contains(w)) {
        auto it = clients.emplace(w, ClientHolder(Client(w)));
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

    Monitor* m = t.getHolder();


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


    if (topicnumber >= this->getTopicCount())
        return this->topics[0].getPointer();

    else
        return this->topics[topicnumber].getPointer();


}


auto WindowManagerModel::getFocusedMon() const -> decltype(focusedmon) {
    return focusedmon;
}




auto WindowManagerModel::registerMonitors(std::vector<std::tuple<Dimensions, Position, u_int>> monitors) -> void {

    for( auto& a : monitors) {
        this->monitors.emplace_back(MonitorHolder(std::move(Monitor(std::get<0>(a), std::get<1>(a), std::get<2>(a)))));
    }


}


auto WindowManagerModel::getMonitor(u_int Monitornumber) const -> Monitor* {

    if (Monitornumber >= monitors.size())
        return monitors[0].getPointer();
    else 
        return monitors[Monitornumber].getPointer();


}


auto WindowManagerModel::getMonitorCount() const -> decltype(monitors.size()) {
    return monitors.size();
}




auto WindowManagerModel::unmanageWindow(Window w) -> void {

    auto it = clients.find(w);


    if (it == clients.end() ) {
        return;
    }

    else
        clients.erase(it);


}


auto WindowManagerModel::registerRules(std::vector<Rule> r) -> void {



    for(auto& rule : r)
        rules.emplace_back(RuleHolder(std::move(rule)));
}


auto WindowManagerModel::getRules() -> decltype(rules)& {
    return rules;
}
<<<<<<< Updated upstream
=======
<<<<<<< Updated upstream
=======
>>>>>>> Stashed changes

auto WindowManagerModel::getGap() const -> decltype(windowgaps) {
    return windowgaps;
}



auto WindowManagerModel::loadConfig() -> void {

    auto& R = ConfigReader::getInstance();

    if (R.empty) {
        //TODO: Handle Config not read
    }


    auto& C = R._configs;

    this->windowgaps = C.windowgap;


    registerTopics(C.topicnames);

    registerRules(C.rules);


    if (getMonitorCount() > getTopicCount()) {
        //TODO: Errorhandling when not enough topics
    }

    int i = 0;
    for(auto& t : topics) {
        t.get().setHolder(monitors[i].getPointer());
        monitors[i].get().setCurrent(t.getPointer());
        i++;
    }



}
<<<<<<< Updated upstream
=======
>>>>>>> Stashed changes
>>>>>>> Stashed changes
