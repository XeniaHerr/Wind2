#ifndef MONITOR_H
#define MONITOR_H


#include "structs.h"
#include <sys/types.h>
#include <ArrangerSelector.h>

namespace Wind {


    class Topic;
    class Client;


    /**
     * @class Monitor
     *
     * @brief Representation of a physical Display
     *
     * Each active Display has an instance of this Class belongin to it. Every Monitor must display a Topic at all times. 
     * All X11 functions are acceccing Topics via the monitor they are beloning to.*/
    class Monitor {
        public:
            Monitor();

            Monitor(u_int32_t x, u_int32_t y);


            Monitor(Dimensions d, Position p);

            Monitor(Dimensions d, Position p, u_int8_t barHeight);


            Monitor(Monitor&& other);



             void display(Topic* other); //What should this function do ??


             /*
              * @brief arrange the current Topic
              *
              * Gets an Instance of a ConcreteArranger and uses it to calculate
              * targetPositions and TargetDimensions. Calls @see adjustforGaps.
              * Doesn't render Clients
              * */
            void arrange();

            Dimensions getDimensions() const;

            Position getPosition() const;

            Topic* getCurrent();

            void setCurrent(Topic *topic);


            /**
             * @brief toggles the Bar visibillity
             *
             * Swaps real and usableDimensions. May be prone to errors by vertivally stacked displays*/
            void toggleBar();


            void setBarHeight(u_int8_t h);

            void setArranger(AbstractArranger* a);


            void setLayout(u_int16_t index);

            void nextLayout();

            void prevLayout();

        private:

            Topic* current;  /**< Currently displayed topic, may not be null*/

            Dimensions realDimensions; /**< Pixelcount of the Physical Display*/


            Dimensions usableDimensions; /**< Area usable for Clients*/


            Position realPosition; /**< Coordinate of the top left pixel in relation to the origin*/

            Position usablePosition; /**< Coordinate of the top left pixel in regards to usable area*/



            bool is_active; //Where do i want to use this variable?


            u_int8_t barHeight = 0; /**< Barheight TODO: SHould this belong to config?*/




            ArrangerSelector layouts; /**< Layout Manager*/



            void adjustforBarHeight();

            /**
             * @brief recalcualtes Position and Dimensions accoring to windowgaps
             *
             * The Arranger returns the Dimensions and Positions of the frame the window can 
             * occupy. the gaps are also part of this frame, which means the real Values must be smaller*/
            void adjustforGaps( Dimensions& d, Position& p);




    };
}


#endif /*MONITOR_H*/

