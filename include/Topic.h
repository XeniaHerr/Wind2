#include <X11/X.h>
#include <memory>
#include <string>
#include <sys/types.h>
#include <list>



namespace Wind {

class Client;

class Monitor;

struct Dimensions;

    class Topic {

        public:

            Topic();


//            std::shared_ptr<Client> operator[](u_int16_t i) {
//                if (i <= clients.size()) {
//                    return std::make_shared<Client>(*clients[i]);
//                }
//
//                return nullptr;
//            }


            std::list<Client*>& get_clients() {
                return clients;
            }


            void send_to_topic(Topic & t, Client * c);


            void adopt( Client& c) {
                clients.push_front(&c);
            }

            const Client& abandon( Client& c) {
                        clients.remove(&c);
                        return c;
            }


            void renderclients();

            void setHolder(Monitor* mon) {
                holder = mon;
            }


            Monitor& getHolder() const { return *holder;}

        private:

            std::string name;

            Client* focusedclient;

            Monitor* holder;


            float master_fact;
            


            std::list<Client*> clients;


            


    };
}
