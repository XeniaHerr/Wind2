
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
            std::vector<Client*> filterClients(P&& predicate) {
                std::vector<Client*> ret;
                for ( auto &a : clients) {
                    if (predicate(a))
                        ret.push_back(&a.second.get());
                }
                return ret;
            }


        void moveClienttoTopic(Window w, u_int topicnumber);


        void moveTopictoMonitor(Topic& topic, Monitor& monitor);





        void manageWindow(Window w);


        void unmanageWindow(Window w);


        void arrangeAllMonitors() {
            for (auto& a : monitors) {
                a.get().arrange();
            }
        }

        private:

        WindowManagerModel() {}


        std::map<Window, ClientHolder> clients;


        std::vector<TopicHolder> topics;

        std::vector<MonitorHolder> monitors;



        Monitor* focusedmon;




    };


}

#endif /*WINDOWMANAGERMODEL_H*/
