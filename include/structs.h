

#ifndef STRUCTS_H
#define STRUCTS_H

#include <sys/types.h>
namespace Wind {

    struct Dimensions {

        u_int32_t x;
        u_int32_t y;


        Dimensions(u_int32_t _x, u_int32_t _y) : x(_x), y(_y)  {} 

        Dimensions() : x(0), y(0) {}
    };
}


#endif /*STRUCTS_H*/
