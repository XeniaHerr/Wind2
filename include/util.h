


#include <X11/X.h>
#include <map>
#include <string>




class Keylookup {


    struct keysym_table {
        KeySym sym;
        std::string str;
    };

    std::map<std::string, KeySym> lookup;



    Keylookup() {

        if( lookup.empty()) {


        }
    }
};
