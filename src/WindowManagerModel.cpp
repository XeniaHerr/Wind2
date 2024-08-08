
#include <WindowManagerModel.h>
#include <X11/X.h>
#include <system_error>


using namespace Wind;

auto WindowManagerModel::moveClienttoTopic(Window w, u_int topicnumber) -> void {

        if (!clients.contains(w))
            return;
        if (topics[topicnumber].get() == clients[w].get().getOwner())
        //if (topics[topicnumber].getPointer().get() == clients[w].getPointer().get()->getOwner())
            return;

        clients[w].get().getOwner().releaseOwnership(clients[w].get());
        topics[topicnumber].get().takeOwnership(clients[w].get());
    }



auto moveTopictoMonitor(Topic& topic, Monitor& monitor) -> void {


        if (&topic.getHolder() == &monitor) return;

        if (monitor.getCurrent() != nullptr) {

            Monitor* orig = &topic.getHolder();

            auto old = monitor.getCurrent();

            old->setHolder(orig);

            orig->getCurrent() = old;

            

        }

            monitor.getCurrent() = std::make_shared<Topic> (topic);

            topic.setHolder(&monitor);
    }



    auto WindowManagerModel::manageWindow(Window w) -> void {

//        if (!clients.contains(w)) {clients.emplace(w, ClientHolder(Client(w)));
//        }

    }
