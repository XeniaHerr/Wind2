#include "structs.h"
#include <ConcreteArranger.h>
#include <sys/types.h>
#include <Monitor.h>



using namespace Wind;

auto Monocle::getDimensions(Monitor& m, u_int16_t position, u_int16_t max) -> Dimensions {


    Dimensions d = m.getDimensions();


    return d;

}



auto Monocle::getPosition(Monitor& m, u_int16_t position, u_int16_t max) -> Position {

    Position p = m.getPosition();
    return p;
}




