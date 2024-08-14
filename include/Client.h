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


            /*
             * @brief Constructor
             *
             * @param win X11 Window ID
             *
             * A Client shouldn't exist without a underlying Window
             * */
            Client(Window win);

            /**
             * @brief Move Constructor, nececcary for Holder Initializsation*/
            Client(const Client&& other);

            ~Client() {
                //Kill window
            }

            void sendevent();

            XWindowAttributes get_Window_Attributes();


            /**
             * @brief Make Client fullscreen*/
            void setFullscreen();

            /**
             * @breif Set New Dimensions
             * @param dimensions new Dimensions*/
            void setDimensions(Dimensions dimensions);

            /**
             * @brief Set New Dimensions
             *
             * Alternative Overload takes raw Dimentions instead of a Dimensions
             *
             * @param x x Dimension
             * @param y y Dimension*/
            void setDimensions(u_int32_t x, u_int32_t y);


            void setTargetDimensions(Dimensions d);

            void setTargetPositions(Position p);

            /**
             * @brief set New position
             *
             * @param pos New Position*/
            void setPosition(Position pos);


            void setOwner(Topic& t) ;


            void setVisible(bool t);



            /**
             * @brief toggle the Floating Status
             *
             * Toggles the is_floating variable. */
            void toggleFloating();

            /* Getter Function*/

            /**
             * @breif get the currentPosition
             * @returns currentPosition*/
            Position getPosition() const;


            /**
             * @brief get the oldPosition
             * @returns oldPosition*/
            Position getOldPosition() const;


            /**
             * @brief get the current Owner
             * @returns reference to _owner*/
            Topic& getOwner() const; 

            /**
             * @brief get the currentDimension
             * @returns currentDimension*/
            Dimensions getCurrentDimensions() const; 

            /**
             * @brief get the last Dimensions
             * @returns oldDimension*/
            Dimensions getOldDimensions() const;


            Dimensions getTargetDimension() const;

            Position getTargetPosition() const;

            Window getWindow() const;

            bool isOrphan() const;

            bool isFloating() const;

            bool isVisible() const;


        private:

            Window _window; /**< X11 Window ID */

            Topic* _owner; /**< Currrent Topic*/


            Dimensions currentDimension, /**< Current Dimensions(width, height)*/
                       oldDimension, /**< Last Dimensions(width, height)*/
                       targetDimension; /**< Dimensions Window should be resized to */


            Position currentPosition, /**< Current Position(x,y)*/
                     oldPosition, /**< Last Position(x,y)*/
                     targetPosition; /**< Position the Window should be moved to*/


            bool is_floating, /**< Floating Status*/ 
                 is_visible,  /**< Visibility status*/
                 is_fullscreen,  /**Fullscreen Status*/
                 is_orphan; /**< Has Owner?*/





    };


}








#endif /*CLIENT_H*/
