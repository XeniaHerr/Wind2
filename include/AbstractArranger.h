
#include "WindowManagerModel.h"
#include "structs.h"
#include <sys/types.h>


using namespace Wind;

/**
 * @file Interface for Monitor arrangers*/
class AbstractArranger {

    public:


        virtual void arrange(Monitor m) = 0;

        virtual Dimensions getDimensions(Monitor m, u_int16_t position) = 0;





};
