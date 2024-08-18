#ifndef CLIENT_H
#define CLIENT_H
#include "Rules.h"
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
                //Killing the window is not the responsibillity of the client. It should be done by in the unmanage function of the Run class
            }

            void sendevent();

            XWindowAttributes get_Window_Attributes();


            /**
             * @brief Make Client fullscreen*/
            void setFullscreen();

            /**
             * @brief Set New Dimensions
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

            void setName(std::string name);

            void setClass(std::string wclass);

            void setType(Windowtype t);



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


            /**
             * @brief get the next Dimenstion
             * @returns targetDimension*/
            Dimensions getTargetDimension() const;

            /**
             * @brief get the next Position
             * @returns targetPosition*/
            Position getTargetPosition() const;

            /**
             * @brief get the X11 WindowID
             * @returns window*/
            Window getWindow() const;


            /**
             * @brief get the WindowName
             * @returns Name*/
            std::string getName() const;

            /**
             * @brief get the Windowclass
             * @returns windowclass*/
            std::string getClass() const;

            /**
             * @brief get the Windowtype
             * @returns type*/
            Windowtype getType() const;

            /**
             * @brief Checks wether the Window has a Owner*/
            bool isOrphan() const;

            /**
             * @brief check if the Window is floating*/
            bool isFloating() const;

            /**
             * @brief check if the Window is visible*/
            bool isVisible() const;


            /**
             * @brief apply the rule
             *
             * Adjust local Settings according to the rules it follows*/
            void applyRule();

            /**
             * @brief Set current rule*/
            void setRule(Rule::RuleContent r);

            /**
             * @brief get current rule
             * @reurns rules*/
            const Rule::RuleContent getRule() const;

            /**
             * @brief Search for eligble rule
             * Search all ruls and find the rule that fits the best. Defaults to the dummyrule*/
            void attachRule();

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

            Rule::RuleContent rules;


            std::string name; /**< Name of the Window*/

            std::string windowclass; /**< Class the Window belongs to*/

            Windowtype type; /**< Type of the Window @see Windowtype*/







    };


}








#endif /*CLIENT_H*/
