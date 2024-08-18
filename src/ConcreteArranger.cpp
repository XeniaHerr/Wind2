#include "structs.h"
#include <ConcreteArranger.h>
#include <sys/types.h>
#include <Monitor.h>
#include <Topic.h>



using namespace Wind;

auto Monocle::getDimensions(Monitor& m, u_int16_t position, u_int16_t max) -> Dimensions {


    Dimensions d = m.getDimensions();


    return d;

}



auto Monocle::getPosition(Monitor& m, u_int16_t position, u_int16_t max) -> Position {

    Position p = m.getPosition();
    return p;
}


auto Monocle::name() -> std::string {
    return "Monocle";
}


auto MasterTile::getDimensions(Monitor& m, u_int16_t position, u_int16_t max) -> Dimensions {

    if (max == 1) 
        return m.getDimensions();


    Dimensions ret;

    ret.width = m.getCurrent()->getMasterfact() * m.getDimensions().width;

    if (position == 1) {
        ret.height = m.getDimensions().height;
        return ret;
    }


    ret.width = m.getDimensions().width - ret.width;


    ret.height = m.getDimensions().height / (max -1);


    return ret;
}


auto MasterTile::getPosition(Monitor& m, u_int16_t position, u_int16_t max) -> Position {

    if (max == 1 || position == 1)
        return m.getPosition();

    Position p(m.getPosition());

    p.x += m.getCurrent()->getMasterfact() * m.getDimensions().width;


    p.y += (m.getDimensions().height /( max -1))  * (position - 2);
    return p;
}



auto MasterTile::name() -> std::string {
    return "MasterTile";
}





