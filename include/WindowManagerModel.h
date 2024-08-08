
#ifndef WINDOWMANAGERMODEL_H
#define WINDOWMANAGERMODEL_H

#include <X11/X.h>
#include <map>
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


        /**Systemfunctions*/

        void moveClienttoTopic(Window w, u_int topicnumber);
        void moveClienttoTopic(Window w, Topic& t);


        void moveTopictoMonitor(Topic& topic, Monitor& monitor);





        void manageWindow(Window w);


        void unmanageWindow(Window w);

        void registerTopics(std::vector<Topic>&& topics);

        void registerMonitors(std::vector<Monitor>&& monitors);

        void focusClient(Window w);



        Topic* getTopic(u_int topicnumber) const;


        Monitor* getMonitor(u_int monitornumber) const;

        void arrangeAllMonitors() {
            for (auto& a : monitors) {
                a.get().arrange();
            }
        }

        /**Helperfunctions*/
        u_int getClientCount();


        Client* getClient(Window w) const ;


        template<typename P> requires std::predicate<P>
            std::vector<Client*> filterClients(P&& predicate) {
                std::vector<Client*> ret;
                for ( auto &a : clients) {
                    if (predicate(a))
                        ret.push_back(&a.second.get());
                }
                return ret;
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
