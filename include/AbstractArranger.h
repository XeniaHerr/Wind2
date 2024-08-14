#ifndef ABSTRACTARRANGER_H
#define ABSTRACTARRANGER_H
#include "structs.h"
#include <sys/types.h>
#include <vector>

/**
 * @file Interface for Monitor arrangers*/


namespace Wind {
    class Monitor;

    class AbstractArranger {

        public:


          //  virtual void arrange(Monitor& m) = 0;

            virtual Dimensions getDimensions(Monitor& m, u_int16_t position, u_int16_t max) = 0;

            virtual Position getPosition(Monitor& m, u_int16_t position, u_int16_t max) = 0;

            virtual ~AbstractArranger() = default;





    };









}
#endif /*ABSTRACTARRANGER_H*/
