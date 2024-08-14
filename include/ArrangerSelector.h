
#include "AbstractArranger.h"
#include <ConcreteArranger.h>



namespace Wind {
class ArrangerSelector {

    public:

        AbstractArranger* getArranger() const;

        AbstractArranger* next();
        AbstractArranger* previous();

        ArrangerSelector();
        ~ArrangerSelector();



    private:
        std::vector<AbstractArranger*> _arrangers;


        u_int _index;


};


}

