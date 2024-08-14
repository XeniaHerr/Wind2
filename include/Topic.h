#ifndef TOPIC_H
#define TOPIC_H
#include "AbstractArranger.h"
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
//
            Topic(Topic&& other);
//            Topic(Topic& other);


            std::list<Client*>& getClients();


            void send_to_topic(Topic & t, Client * c);


            void takeOwnership( Client& c); //DESIGN CHOICE: Client must always beong to a monitor. The only exception to that is at the beginning of its lifecycle. I hav therefore removed the function abandon, because it violates this principle. But i still have to give the method releaseOwnership, because i need to remove the client from the clientlist.

            const Client& releaseOwnership( Client& c);


            void renderclients();

            void setHolder(Monitor* mon);

            Monitor* getHolder() const; 

            std::string getName() const;

            void setFocus(Client *client);
            void setFocus(Client &client);



            bool operator==(const Topic& other) const {
                return this->name == other.name;
            }


            Client* getFocused() const ;





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
