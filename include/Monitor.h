#include "structs.h"
#include <memory>
#include <sys/types.h>
#include <system_error>

namespace Wind {


    class Topic;


    class Monitor {
        public:
            Monitor();

            Monitor(u_int32_t x, u_int32_t y);


            Monitor(Dimensions d, Position p);



             void display(Topic* other);


            void arrange();

            Dimensions getDimensions() const;

            Position getPosition() const;

            std::weak_ptr<Topic> getCurrent();

            void setCurrent(std::weak_ptr<Topic> topic);

        private:

            std::weak_ptr<Topic> current;

            Dimensions realDimensions;


            Dimensions usableDimensions;


            Position realPosition;

            Position usablePosition;



            bool is_active;


            u_int8_t barHeight = 0; /**TODO: Decide when and how to set*/



    };
}



/**
 * Decicion: Maybe i should create a Factory for Monitor and Builder. Probably not, because monitors are not all that different from each other*/
