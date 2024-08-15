#ifndef ARRANGERSELECTOR_H
#define ARRANGERSELECTOR_H
#include "AbstractArranger.h"
#include <ConcreteArranger.h>



namespace Wind {
class ArrangerSelector {

    public:

        AbstractArranger& getArranger();

        AbstractArranger& next();
        AbstractArranger& previous();

        ArrangerSelector();
        ~ArrangerSelector();

        AbstractArranger& operator++();

        AbstractArranger& operator--();



    private:
        std::vector<AbstractArranger*> _arrangers;


        u_int _index;


};


}

#endif /*ARRANGERSELECTOR_H*/
