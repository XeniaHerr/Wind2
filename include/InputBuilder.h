#ifndef KEYBUILDER_H
#define KEYBUILDER_H

#include <Inputstructs.h>
#include <X11/X.h>





extern unsigned long _internal_choosen_modifier;


namespace Wind {




class KeyBuilder {

    public:
        Key finish();
        KeyBuilder& setShiftMask();
        KeyBuilder& setControlMask();
        KeyBuilder& setModMask();

        KeyBuilder& setKeySym(KeySym k);


    private:

        Key k;

};


#endif /*KEYBUILDER_H*/


}
