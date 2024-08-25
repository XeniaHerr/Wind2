
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


        /**
         * @rief Singleton method*/
        static WindowManagerModel& getInstance() {

            static WindowManagerModel s;
            return s;

        }

        WindowManagerModel(const WindowManagerModel& other) = delete;
        WindowManagerModel(const WindowManagerModel&& other) = delete;

        WindowManagerModel& operator=(const WindowManagerModel& other) = delete;
        WindowManagerModel& operator=(const WindowManagerModel&& other) = delete;


        /**Systemfunctions*/

        /**
         * @brief move Client to Topic
         *
         * Looksup Client for w and moves it ot topic belongig to topicnumber*/
        void moveClienttoTopic(Window w, u_int topicnumber);
        void moveClienttoTopic(Window w, Topic& t);


        /**
         * @brief Display topic on monitor
         *
         * If topic is already displayed on a Monitor they are swapped instead*/
        void moveTopictoMonitor(Topic& topic, Monitor& monitor);


        /**
         * TODO: Implement*/
        void resizeClient(Window w, Dimensions d);

        /**
         * TODO: Implement*/
        void moveClient(Window w, Position p);

        //void terminateClient(Window w);


        /**
         * @brief Copy config values
         *
         * Must be called after @see registerMonitors.
         * @callgraph*/
        bool loadConfig();

        /**
         * @brief reloads Config values
         *
         * Replaces old values with new Values. Migrates Clients from Old Topics to new Topics. If a topic gets deleted
         * , then the clients are moved to the first Topic
         * @callgraph*/
        void reloadConfig();






        /**
         * @brief Creates Client for w
         *
         * @param w X11 Window id
         *
         * Checks if w is already a Client. If not a new Client is created
	 *
	 * @return nullptr if Window already has Client*/
        Client * manageWindow(Window w);

        /**
         * @brief removes Client
         *
         * Should handle removing client from topic list*/
        void unmanageWindow(Window w);

        /**
         * @brief creates Topics from names
         *
         * Creates new Topics from the provided Names
         * Should check if no duplicates exist*/
        void registerTopics(std::vector<std::string> parameters);

        /**
         * @brief creates Moniors from Prameter list
         *
         * @param monitors Tuple of Dimension, Position and barheight
         *
         * */
        void registerMonitors(std::vector<std::tuple<Dimensions, Position, u_int>> monitors);

        void registerRules(std::vector<Rule> rules_vec);

        /**
         * @brief set Focus to client
         *
         * If Client is not on Monitor currently focused, focus monitor*/
        void focusClient(Window w);



        Topic* getTopic(u_int topicnumber) const;

	Topic* getTopic(std::string);


        Monitor* getMonitor(u_int monitornumber) const;

        /**
         * @brief Call arrange for all Monitors*/
        void arrangeAllMonitors() {
            for (auto& a : monitors) {
                a.get().arrange();
            }
        }

        /**Helperfunctions*/
        u_int getClientCount() const;

        size_t getTopicCount() const;

        size_t getMonitorCount() const;
        u_int16_t getGap() const;


    



        std::vector<RuleHolder>& getRules();


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

        WindowManagerModel(); /**< Konstruktor must be private according to Singleton*/


        std::map<Window, ClientHolder> clients; /**< All Clients*/


        std::vector<TopicHolder> topics; /**< All Topics*/

        std::vector<MonitorHolder> monitors; /**< All monitors*/


        std::vector<RuleHolder> rules; /**< All rules*/



        Monitor* focusedmon; /**< Currenlty focused monitor*/

        u_int16_t windowgaps; /**< Windowgaps, part of Config*/








    };


}

#endif /*WINDOWMANAGERMODEL_H*/
