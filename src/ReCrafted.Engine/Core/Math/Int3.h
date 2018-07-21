// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef INT3_H
#define INT3_H

/**
 * \brief Integer based vector with 3 components - X, Y and Z.
 */
struct Int3
{
public:
    /**
     * \brief Default constructor,
     * sets 0 to all components of this vector.
     */
    Int3() : x(0), y(0), z(0) { }

    /**
     * \brief Sets given values to all components of this vector.
     * \param x 
     * \param y 
     * \param z 
     */
    Int3(const int x, const int y, const int z) : x(x), y(y), z(z) { }

public:
    /**
     * \brief X component.
     */
    int x;

    /**
    * \brief Y component.
    */
    int y;

    /**
    * \brief Z component.
    */
    int z;
};

#endif // INT3_H
