
#include <X11/X.h>
#include <X11/Xlib.h>
#include <structs.h>



class Topic;
namespace Wind {

class Client {

    public:


        Client(Window win);

        ~Client() {
            //Kill window
        }

        void sendevent();

        void updategeom();

        XWindowAttributes get_Window_Attributes();


        void setFullscreen();

        void setCurrentDimensions(Dimensions& dimensions);


        Topic& getOwner() const { return *owner; }


        Dimensions getCurrentDimensions() const { return currentDimension;}

    private:

        Window window;

        Topic* owner;


        Dimensions currentDimension;

        Dimensions oldDimension;


        bool is_floating, is_visible, is_fullscreen;



};


}
