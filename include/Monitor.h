#include "structs.h"
#include <memory>
#include <sys/types.h>

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

            std::shared_ptr<Topic> current;

        private:


            Dimensions realDimensions;


            Dimensions usableDimensions;


            Position realPosition;

            Position usablePosition;



            bool is_active;


            u_int8_t barHeight = 0; /**TODO: Decide when and how to set*/



    };
}
