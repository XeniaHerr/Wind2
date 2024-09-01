#ifndef TOPIC_H
#define TOPIC_H
#include "AbstractArranger.h"
#include <X11/X.h>
#include <concepts>
#include <string>
#include <sys/types.h>
#include <list>



namespace Wind {

class Client;

class Monitor;

struct Dimensions;

    /**
     * @class Topic
     * @brief Representation of a virtual Desktop
     *
     * A Topic is a collection of Clients, that are being displayed together. A Topic can be displayed on a Monitor, but
     * doesn't have to. A Topic can have Clients, but doesn't have to. The clients are not stored here, but in the WindowManagerModel.
     * @see WindowManagerModel*/
    class Topic {

        public:

            Topic(std::string name);


            Topic(Topic&& other);

	    Topic(Topic& other)  = delete;
	    Topic& operator=(Topic& other) = delete;
	    Topic& operator=(Topic&& other) = delete;


            /**
             * @brief get Clients belonging to Topic
             * @return clients list reference to Clientpointer*/
            std::list<Client*>& getClients();


            /**
             * @deprecated*/
            void send_to_topic(Topic & t, Client * c);


            void takeOwnership( Client& c); //DESIGN CHOICE: Client must always beong to a monitor. The only exception to that is at the beginning of its lifecycle. I hav therefore removed the function abandon, because it violates this principle. But i still have to give the method releaseOwnership, because i need to remove the client from the clientlist.

            /**
             * @brief remove Client from Topic
             *
             * Doesn't change the Client's view, because every client must belong to a topic*/
            const Client& releaseOwnership( Client& c);


            /**
             * @deprecated*/
            void renderclients();

            /**
             * @brief set Holder
             * @param mon Pointer to Monitor*/
            void setHolder(Monitor* mon);

            Monitor* getHolder() const; 

            std::string getName() const;

            bool setFocus(Client *client);
            bool setFocus(Client &client);

            void setMasterfact(float f);

            float getMasterfact() const;



            bool operator==(const Topic& other) const {
                return this->name == other.name;
            }


            Client* getFocused() const ;







        private:

            std::string name; /**< Name of the Topic, muust be unique*/

            Client* focusedclient; /**< Client that has the focus, may be null*/


            Monitor* holder; /**< Monitor Topic is currenlty displayed, may be null*/


            float master_fact; /**< Master fact, muste be between 0 and 1*/

            


            /**
             * @brief Clients belongig to the Topic
             *
             * List because easier poping and pushing. May also serve as a focus stack. TODO: Implement focus stack*/
            std::list<Client*> clients;

            /*
             * @brief chek wether client belongs to the topic
             * */
        inline bool containsClient(Client& client) const {
            for (auto& c : clients) {
                if(c == &client)
                    return true;
            }
            return false;
        };

            


    };
}


#endif /*TOPIC_H*/
