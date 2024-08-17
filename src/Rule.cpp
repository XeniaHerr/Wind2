#include "structs.h"
#include <Rules.h>
#include <sys/types.h>



using namespace Wind;

Rule::Rule() {};




auto Rule::isApplicable(string name, string wclass, Windowtype type) -> u_int {

    u_int ret = 0;

    if (type == this->type)
        ret = 1;
    if ( wclass == this->windowclass)
        ret = 2;
    if (name == this->windowname)
        ret = 3;

    return ret;
}
