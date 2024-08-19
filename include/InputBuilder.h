#ifndef KEYBUILDER_H
#define KEYBUILDER_H

#include <Inputstructs.h>
#include <X11/X.h>







namespace Wind {


    unsigned long _internal_choosen_modifier;


class KeyBuilder {

    public:
        Key finish();
        KeyBuilder& setShiftMask();
        KeyBuilder& setControlMask();
        KeyBuilder& setModMask();

        KeyBuilder& setKeySym(KeySym k);

        template<typename Func, typename...Args>
        KeyBuilder& setAction( Func&& f, Args...args) {
            k.registerFunc(f, args...);

            return *this;
        }

    private:

        Key k;

};


#endif /*KEYBUILDER_H*/


}
