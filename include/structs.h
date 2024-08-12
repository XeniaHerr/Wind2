#ifndef STRUCTS_H
#define STRUCTS_H
#include <sys/types.h>

/**
 * @file This file holds the Structs Dimensions and Position. The reason these are combined
 * here instead of being in seperate files is because these concepts are mostly used together.
 * ALso they are only amalgamations of two values each, so it is simply not worth to have 
 * seperate files.*/


namespace Wind {

    /**
     * @brief Representation of the Dimensions of an Object.*/
    struct Dimensions {

        u_int32_t width; /**< size in the x direction*/
        u_int32_t height; /**< size in the y direction*/


        /**
         * @ brief Value Constructor*/
        Dimensions(u_int32_t _width, u_int32_t _height) : width(_width), height(_height)  {} 

        /**
         * @brief Default konstruktor*/
        Dimensions() : width(0), height(0) {}


        /**
         * @brief Comparison operator
         *
         * Two Dimensions are equal of both of width and height match*/
        bool operator==(const Dimensions& other)  const {
            return this->width == other.width && this->height == other.height;
        }
    };

    /**
     * @brief Representation of the Position of an Object
     *
     * The Position is the coordinate of the upper left corner. Like in X11 customary
     * the origin is the upper left corner of the screen, and the y axis is inverted*/
    struct Position {
        u_int32_t x; /**< x Position*/
        u_int32_t y; /**< y Position*/


        /**
         * @brief Value Constructor*/
        Position(u_int32_t x, u_int32_t y) : x(x), y(y) {}

        /**
         * @brief Default Constructor*/
        Position() : x(0), y(0) {}


        /**
         * @brief Comparison operator
         *
         * Two Positions are equal if the coordinates they represent are the same*/
        bool operator==(const Position& other) const {
            return this->x == other.x && this->y == other.y;
        }
    };
}





#endif /*STRUCTS_H*/
