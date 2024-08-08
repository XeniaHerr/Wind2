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

            Monitor(Dimensions d, Position p, u_int8_t barHeight);



             void display(Topic* other); //What should this function do ??


            void arrange();

            Dimensions getDimensions() const;

            Position getPosition() const;

            std::shared_ptr<Wind::Topic> getCurrent();

            void setCurrent(std::shared_ptr<Topic> topic);


            void toggleBar();


            void setBarHeight(u_int8_t h);

        private:

            std::shared_ptr<Topic> current;

            Dimensions realDimensions;


            Dimensions usableDimensions;


            Position realPosition;

            Position usablePosition;



            bool is_active; //Where do i want to use this variable?


            u_int8_t barHeight = 0; /**TODO: Decide when and how to set, set to 0 for testing*/



    };
}



/**
 * Decicion: Maybe i should create a Factory for Monitor and Builder. Probably not, because monitors are not all that different from each other*/
