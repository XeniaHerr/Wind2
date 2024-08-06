#include "structs.h"
#include <memory>
#include <sys/types.h>

namespace Wind {


    class Topic;


    class Monitor {
        public:
            Monitor();

            Monitor(u_int32_t x, u_int32_t y) : realDimensions(x,y) {

                usableDimensions.x = x;
                usableDimensions.y = y; //DO some calculations first;
                
                is_active = false;
            }


             void display(Topic* other);


            void arrange();

            Dimensions getDimensions() const {return usableDimensions;}

            std::shared_ptr<Topic> current;

        private:


            Dimensions realDimensions;


            Dimensions usableDimensions;



            bool is_active;


            u_int8_t barHeight;



    };
}
