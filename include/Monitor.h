#include "structs.h"
#include <memory>
#include <sys/types.h>

namespace Wind {


    class Topic;


    class Monitor {
        public:
            Monitor();

            Monitor(u_int32_t x, u_int32_t y);



             void display(Topic* other);


            void arrange();

            Dimensions getDimensions() const;

            std::shared_ptr<Topic> current;

        private:


            Dimensions realDimensions;


            Dimensions usableDimensions;



            bool is_active;


            u_int8_t barHeight;



    };
}
