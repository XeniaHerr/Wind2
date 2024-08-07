#ifndef STRUCTS_H
#define STRUCTS_H
#include <sys/types.h>
namespace Wind {

    struct Dimensions {

        u_int32_t width;
        u_int32_t height;


        Dimensions(u_int32_t _width, u_int32_t _height) : width(_width), height(_height)  {} 

        Dimensions() : width(0), height(0) {}


        bool operator==(const Dimensions& other)  const {
            return this->width == other.width && this->height == other.height;
        }
    };

    struct Position {
        u_int32_t x;
        u_int32_t y;


        Position(u_int32_t x, u_int32_t y) : x(x), y(y) {}
        Position() : x(0), y(0) {}


        bool operator==(const Position& other) const {
            return this->x == other.x && this->y == other.y;
        }
    };
}





#endif /*STRUCTS_H*/
