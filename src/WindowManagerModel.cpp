#include "structs.h"
#include <WindowManagerModel.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <sys/types.h>
#include <utility>
#include <Rules.h>
#include <RuleBuilder.h>
#include <ConfigReader.h>
#include <Logger.h>



using namespace Wind;


 WindowManagerModel::WindowManagerModel() {


     this->focusedmon = nullptr;

     this->windowgaps = 0;

    //Create Default rule that applies to all clients
    
    rules.emplace_back(RuleHolder(RuleBuilder().finish()));

}

auto WindowManagerModel::moveClienttoTopic(Window w, u_int topicnumber) -> void {

    auto& Log = Logger::GetInstance();

        if (!clients.contains(w) || topicnumber > getTopicCount())
            return;
        Log.Info("Cleared first guard");
        if (&topics[topicnumber].get() == &clients[w].get().getOwner())
            return;

        Log.Info("Cleared second guard");
        if (!clients[w].get().isOrphan())
            clients[w].get().getOwner().releaseOwnership(clients[w].get());
        Log.Info("Released by previous owner\n");
        topics[topicnumber].get().takeOwnership(clients[w].get());
        Log.Info("Adopted by new owner");
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



auto WindowManagerModel::manageWindow(Window w) -> Client* {

    if (!clients.contains(w)) {
        auto it = clients.emplace(w, ClientHolder(Client(w)));

	return it.first->second.getPointer();
    }
    return nullptr;

}


auto WindowManagerModel::getClientCount() const  -> u_int {
    return clients.size();
}


auto WindowManagerModel::focusClient(Window w) -> bool {

    auto client = clients.find(w);


    if (client == clients.end())
        return false;

    Client& c = client->second.get();


    Topic& t = c.getOwner();

    Monitor* m = t.getHolder();


    if (m != this->focusedmon)
        focusedmon = m;


    return t.setFocus(c);
}

auto WindowManagerModel::focusClient()-> bool {
    auto& Log = Logger::GetInstance();
    Log.Info("Inside focusClient");

    if (!focusedmon)
	focusedmon = monitors[0].getPointer();

    if (focusedmon->getCurrent()->getFocused() == nullptr) {
	Log.Info("Currently now Client focused, searching for next valid");
	if (focusedmon->getCurrent()->getClients().size() > 0)  {
	    Log.Info("Found client to focus");
	    return focusedmon->getCurrent()->setFocus(focusedmon->getCurrent()->getClients().front());

	}
	else
	    return focusedmon->getCurrent()->setFocus(nullptr);

	}
    return false;
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

auto WindowManagerModel::getGap() const -> decltype(windowgaps) {
    return windowgaps;
}



auto WindowManagerModel::loadConfig() -> bool {

    auto& R = ConfigReader::getInstance();

    auto& Log = Logger::GetInstance();

    Log.Info("Applying the Config");

    if (R.empty) {
        //TODO: Handle Config not read
    }


    auto& C = R._configs;

    this->windowgaps = C.windowgap;

    Log.Info("Read Windowgaps");


    registerTopics(C.topicnames);

    Log.Info("Registered Topics");

    registerRules(C.rules);

    Log.Info("Registered Rules");

    //Log.Info("Check if Monitors {} <= {} Topics", getMonitorCount(), getTopicCount());

    if (getMonitorCount() > getTopicCount()) {
        //TODO: Errorhandling when not enough topics
        Logger::GetInstance().Error("Not enough Topics. Need at least {} Topics!", getTopicCount());
	return false;

    }

    Log.Info("Enough Topics found");

    

    int i = 0;

    for(auto& m : monitors) {
	m.get().setCurrent(topics[i].getPointer());
	topics[i].get().setHolder(m.getPointer());
	i++;
    }

    return true;
}


auto WindowManagerModel::reloadConfig() -> void {

    auto& Log = Logger::GetInstance();

    Log.Info("Inside reloadConfig");


    auto& R = ConfigReader::getInstance();

    auto& C = R._configs;


    this->windowgaps = C.windowgap;


    while(!this->rules.empty())
        this->rules.pop_back();

    registerRules(C.rules);


    Log.Info("Rules updated");

    for (auto& c: clients) 
        c.second.get().attachRule();

    std::vector<TopicHolder> localtopics;

    for ( auto name : C.topicnames)
        localtopics.emplace_back(TopicHolder(std::move(Topic(name))));


    Log.Info("New topics created");

    auto contains = [&](std::vector<TopicHolder>& vec, std::string name) {
        for (auto& a : vec)
            if (a.get().getName() == name) {
                Log.Info("Found topic {} again", name);
                return a.getPointer();
            }
        return static_cast<Topic*>(nullptr);
    };

    for (auto& t : topics) {
        Topic* newt;
        if ( (newt = contains(localtopics, t.get().getName()))) {
            Log.Info("Have to move {} clients", t.get().getClients().size());
            while (!t.get().getClients().empty()) {
                Client *c = t.get().getClients().front();
                moveClienttoTopic(c->getWindow(),*newt);
            }
        }
        else {
                while(!t.get().getClients().empty()) {
                    Client *c = t.get().getClients().front();

                moveClienttoTopic(c->getWindow(),localtopics[0].get());
                }

        }
    }
    
    topics.clear();
    topics = std::move(localtopics);



}


auto WindowManagerModel::getTopic(std::string name) -> Topic* {

    for (auto& a: topics)
	if(a.getPointer()->getName() == name)
	    return a.getPointer();
    return nullptr;
}
