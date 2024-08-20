#ifndef INPUTCLASSES_H
#define INPUTCLASSES_H

#include <X11/X.h>
#include <X11/keysym.h>
#include <functional>


namespace Wind {


    struct Modifier {
        unsigned long modifier;
    };

    struct Key : Modifier {
        KeySym keysym;

    };


    struct Button  : Modifier {
        unsigned long button;

    };




}
namespace std {


    template<>
        struct less<Wind::Key> {
            bool operator()(const Wind::Key& lhs, const Wind::Key& rhs) const {
                if (lhs.keysym == rhs.keysym)
                    return lhs.modifier < rhs.modifier;
                else
                    return lhs.keysym < rhs.keysym;
            }
        };

    template<>
        struct less<Wind::Button> {
            bool operator()(const Wind::Button& lhs, const Wind::Button& rhs) const {
                if (lhs.button == rhs.button)
                    return lhs.modifier < rhs.modifier;
                else
                    return lhs.button < rhs.button;
            }
        };
}

#endif /*INPUTCLASSES_H*/
