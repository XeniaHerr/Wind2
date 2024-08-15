#ifndef CONCRETEARRANGER_H
#define CONCRETEARRANGER_H

#include <AbstractArranger.h>




namespace Wind {

    class Monocle : public AbstractArranger {


        public:

        Dimensions getDimensions(Monitor& m, u_int16_t position, u_int16_t max) override;

        Position getPosition(Monitor& m, u_int16_t position, u_int16_t max) override;

        ~Monocle() override = default;

    };

    class MasterTile : public AbstractArranger {


        public:

        Dimensions getDimensions(Monitor& m, u_int16_t position, u_int16_t max) override;

        Position getPosition(Monitor& m, u_int16_t position, u_int16_t max) override;

        ~MasterTile() override = default;

    };


}


#endif /*CONCRETEARRANGER_H*/
