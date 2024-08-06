
#include <map>
#include <X11/X.h>
#include <memory>
#include <vector>
#include <Topic.h>




    class Client;

    class Monitor;
class WindowManagerModel {

    private:


        WindowManagerModel();


        std::vector<std::shared_ptr<Client>> clients;
        std::vector<std::shared_ptr<Monitor>> monitors;


        //std::map<Window,Topic> topics;

        Monitor* focused;
        

    public:


};


