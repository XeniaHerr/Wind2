#ifndef MONITOR_H
#define MONITOR_H


#include "structs.h"
#include <sys/types.h>
#include <ArrangerSelector.h>

namespace Wind {


    class Topic;
    class Client;


    class Monitor {
        public:
            Monitor();

            Monitor(u_int32_t x, u_int32_t y);


            Monitor(Dimensions d, Position p);

            Monitor(Dimensions d, Position p, u_int8_t barHeight);


            Monitor(Monitor&& other);



             void display(Topic* other); //What should this function do ??


            void arrange();

            Dimensions getDimensions() const;

            Position getPosition() const;

            Topic* getCurrent();

            void setCurrent(Topic *topic);


            void toggleBar();


            void setBarHeight(u_int8_t h);

            void setArranger(AbstractArranger* a);



        private:

            Topic* current;

            Dimensions realDimensions;


            Dimensions usableDimensions;


            Position realPosition;

            Position usablePosition;



            bool is_active; //Where do i want to use this variable?


            u_int8_t barHeight = 0; /**TODO: Decide when and how to set, set to 0 for testing*/




            ArrangerSelector layouts;



            void adjustforBarHeight();




    };
}


#endif /*MONITOR_H*/

/**
 * Decicion: Maybe i should create a Factory for Monitor and Builder. Probably not, because monitors are not all that different from each other*/
