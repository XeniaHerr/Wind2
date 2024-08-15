
#ifndef WINDOWMANAGERMODEL_H
#define WINDOWMANAGERMODEL_H

#include "structs.h"
#include <X11/X.h>
#include <cstddef>
#include <map>
#include <sys/types.h>
#include <utility>
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
        using RuleHolder = Holder<Rule>;

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


        void resizeClient(Window w, Dimensions d);

        void moveClient(Window w, Position p);

        void terminateClient(Window w);






        void manageWindow(Window w);


        void unmanageWindow(Window w);

        void registerTopics(std::vector<std::string> parameters);

        void registerMonitors(std::vector<std::tuple<Dimensions, Position, u_int>> monitors);

        void focusClient(Window w);



        Topic* getTopic(u_int topicnumber) const;


        Monitor* getMonitor(u_int monitornumber) const;

        void arrangeAllMonitors() {
            for (auto& a : monitors) {
                a.get().arrange();
            }
        }

        /**Helperfunctions*/
        u_int getClientCount() const;

        size_t getTopicCount() const;

        size_t getMonitorCount() const;


        Client* getClient(Window w) const ;

        Monitor* getFocusedMon() const;


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

        WindowManagerModel();


        std::map<Window, ClientHolder> clients;


        std::vector<TopicHolder> topics;

        std::vector<MonitorHolder> monitors;


        std::vector<RuleHolder> rules;



        Monitor* focusedmon;


        void attachRule(Client& c);




    };


}

#endif /*WINDOWMANAGERMODEL_H*/
