#ifndef ABSTRACTARRANGER_H
#define ABSTRACTARRANGER_H
#include "structs.h"
#include <cmath>
#include <sys/types.h>

/**
 * @file Interface for Monitor arrangers*/


namespace Wind {
    class Monitor;

    class AbstractArranger {

        public:


          //  virtual void arrange(Monitor& m) = 0;

            virtual Dimensions getDimensions(Monitor& m, u_int16_t position, u_int16_t max) = 0;

            virtual Position getPosition(Monitor& m, u_int16_t position, u_int16_t max) = 0;

            virtual ~AbstractArranger() {}


            AbstractArranger() : master_count(1), mfact(0.5) {}


        private:

            u_int8_t master_count;

            double_t mfact;



    };









}
#endif /*ABSTRACTARRANGER_H*/
