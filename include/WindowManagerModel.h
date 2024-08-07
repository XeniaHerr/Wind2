
#ifndef WINDOWMANAGERMODEL_H
#define WINDOWMANAGERMODEL_H

#include <X11/X.h>
#include <map>
#include <memory>
#include <sys/types.h>
#include <vector>
#include <Topic.h>
#include <Monitor.h>
#include <Client.h>
#include <Holder.h>
namespace Wind {





    class WindowManagerModel {

//        using ClientHolder = std::shared_ptr<Client>;
//        using MonitorHolder = std::shared_ptr<Monitor>;
//        using TopicHolder = std::shared_ptr<Topic>;
        using ClientHolder = Holder<Client>;
        using MonitorHolder = Holder<Monitor>;
        using TopicHolder = Holder<Topic>;

        public:


    static WindowManagerModel& getInstance() {

        static WindowManagerModel s;
        return s;

        }

    WindowManagerModel(const WindowManagerModel& other) = delete;
    WindowManagerModel(const WindowManagerModel&& other) = delete;

    WindowManagerModel& operator=(const WindowManagerModel& other) = delete;
    WindowManagerModel& operator=(const WindowManagerModel&& other) = delete;

    template<typename P> requires std::predicate<P>
    std::vector<Topic*> filterTopics(P&& predicate) {
        std::vector<Topic*> ret;
        for ( auto &a : topics) {
            if (predicate(a))
                ret.push_back(&a);
        }
        return ret;
    }


    void moveClienttoTopic(Window w, u_int topicnumber) {

        if (!clients.contains(w))
            return;
        if (&topics[topicnumber] == &clients[w].getOwner())
            return;

        clients[w].getOwner().abandon(clients[w]);
        topics[topicnumber].adopt(clients[w]);
    }


    void moveTopictoMonitor(Topic& topic, Monitor& monitor) {




        if (&topic.getHolder() == &monitor) return;

        if (monitor.current != nullptr) {

            Monitor* orig = &topic.getHolder();

            auto old = monitor.current;

            old->setHolder(orig);

            orig->current = old;

            

        }

            monitor.getCurrent() = std::make_shared<Topic> (topic);

            topic.setHolder(&monitor);
    }

    void manageWindow(Window w) {

        if (!clients.contains(w)) { auto it = clients.emplace(w, ClientHolder(Client(w)));
        //focusedmon->current->   adopt(it.first->second);
        }

    }

    void unmanageWindow(Window w);


    void arrangeAllMonitors() {
        for (auto& a : monitors) {
            a.get().arrange();
        }
    }
            
        private:

        WindowManagerModel();


            std::map<::Window, ClientHolder> clients;


            std::vector<TopicHolder> topics;

            std::vector<MonitorHolder> monitors;



            Monitor* focusedmon;




    };


}

#endif /*WINDOWMANAGERMODEL_H*/
