#ifndef CLIENT_H
#define CLIENT_H
#include <X11/X.h>
#include <X11/Xlib.h>
#include <structs.h>
#include <sys/types.h>

#include <Topic.h>



namespace Wind {

/**
 * @class 
 * @brief Modeling of a single window.
 *
 * A Window is a rectangle that belongs to an Application and can be managed more or less independently from other windows.
 * They are here called Clients. In a client the Window Id is stored alongside other metainformation about the window like
 * size and Positon.
 *
 * The main goal of this Class is to hold and manage these Metadata*/
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

        Window _window; /**< X11 Window ID */

        Topic* _owner; /**< Currrent Topic*/


        Dimensions currentDimension; /**< Current Dimensions(width, height)*/

        Dimensions oldDimension; /**< Last Dimensions(width, height)*/


        Position currentPosition; /**< Current Position(x,y)*/

        Position oldPosition; /**< Last Position(x,y)*/


        bool is_floating, /**< Floating Status*/ 
             is_visible,  /**< Visibility status*/
             is_fullscreen,  /**Fullscreen Status*/
             is_orphan; /**< Has Owner?*/





};


}


#endif /*CLIENT_H*/
