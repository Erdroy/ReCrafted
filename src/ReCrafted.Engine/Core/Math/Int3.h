// ReCrafted (c) 2016-2019 Always Too Late

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

    void operator -=(const Int3& r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;
    }

    void operator +=(const Int3& r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
    }

    void operator *=(const int value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    void operator /=(const int value)
    {
        x /= value;
        y /= value;
        z /= value;
    }

    /// <summary>
    /// Result = Vector - Vector[r] operator
    /// </summary>
    Int3 operator -(const Int3& r) const
    {
        return Int3(x - r.x, y - r.y, z - r.z);
    }

    /// <summary>
    /// Result = Vector + Vector[r] operator
    /// </summary>
    Int3 operator +(const Int3& r) const
    {
        return Int3(x + r.x, y + r.y, z + r.z);
    }

    /// <summary>
    /// Result = Vector * Vector[r] operator
    /// </summary>
    Int3 operator *(const Int3& r) const
    {
        return Int3(x * r.x, y * r.y, z * r.z);
    }

    /// <summary>
    /// Result = Vector * int operator
    /// </summary>
    Int3 operator *(const int r) const
    {
        return Int3(x * r, y * r, z * r);
    }

    /// <summary>
    /// Result = Vector / int operator
    /// </summary>
    Int3 operator /(const int r) const
    {
        return Int3(x / r, y / r, z / r);
    }

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
