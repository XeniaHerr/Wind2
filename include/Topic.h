#ifndef TOPIC_H
#define TOPIC_H
#include <X11/X.h>
#include <string>
#include <sys/types.h>
#include <list>



namespace Wind {

class Client;

class Monitor;

struct Dimensions;

    class Topic {

        public:

            Topic(std::string name);


//            std::shared_ptr<Client> operator[](u_int16_t i) {
//                if (i <= clients.size()) {
//                    return std::make_shared<Client>(*clients[i]);
//                }
//
//                return nullptr;
//            }


            std::list<Client*>& getClients();


            void send_to_topic(Topic & t, Client * c);


            void adopt( Client& c);

            const Client& abandon( Client& c);


            void renderclients();

            void setHolder(Monitor* mon);

            Monitor& getHolder() const; 

            std::string getName() const;

        private:

            std::string name;

            Client* focusedclient;

            Monitor* holder;


            float master_fact;
            


            std::list<Client*> clients;

        inline bool containsClient(Client& client) const {
            for (auto c : clients) {
                if(c == &client)
                    return true;
            }
            return false;
        };

            


    };
}


#endif /*TOPIC_H*/
