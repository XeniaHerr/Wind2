#ifndef ABSTRACRENDERER_H
#define ABSTRACRENDERER_H
#include "Monitor.h"
#include <X11_Abstraction.h>


namespace Wind {
    class AbstractRenderer {


	public:


	    //Change the current Dimensions\Positions to the target ones. How exactly (Animations) is left up to the implementation
	    virtual void render(Monitor& m) = 0;

    };


}


#endif /*ABSTRACRENDERER_H*/
