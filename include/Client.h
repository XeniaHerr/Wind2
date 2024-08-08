#ifndef CLIENT_H
#define CLIENT_H
#include <X11/X.h>
#include <X11/Xlib.h>
#include <structs.h>
#include <sys/types.h>

#include <Topic.h>



namespace Wind {

class Client {

    public:


        Client(Window win);

        Client(const Client&& other);

        ~Client() {
            //Kill window
        }

        void sendevent();

        void updategeom();

        XWindowAttributes get_Window_Attributes();


        void setFullscreen();

        void setDimensions(Dimensions dimensions);

        void setDimensions(u_int32_t x, u_int32_t y);


        Topic& getOwner() const; 


        Dimensions getCurrentDimensions() const; 

        Dimensions getOldDimensions() const;

        Position getPosition() const;

        Position getOldPosition() const;
        
        void setPosition(Position pos);

        Window getWindow() const;

        void setOwner(Topic& t) ;

        bool isOrphan() const;

        bool isFloating() const;

        void toggleFloating();

        bool isVisible() const;


    private:

        Window _window;

        Topic* _owner;


        Dimensions currentDimension;

        Dimensions oldDimension;


        Position currentPosition;

        Position oldPosition;


        bool is_floating, is_visible, is_fullscreen, is_orphan;





};


}


#endif /*CLIENT_H*/
