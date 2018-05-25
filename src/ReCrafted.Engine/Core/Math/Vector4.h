// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef Vector4_H
#define Vector4_H

// includes
#include "ReCrafted.h"

struct Vector4i
{
public:
    /// <summary>
    /// Default constructor,
    /// sets 0 to all components of this vector.
    /// </summary>
    Vector4i() : x(0), y(0), z(0), w(0)
    {
    }

    /// <summary>
    /// Sets given values to all components of this vector.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    /// <param name="w"></param>
    Vector4i(int x, int y, int z, int w) : x(x), y(y), z(z), w(w)
    {
    }

public:
    /// <summary>
    /// X component
    /// </summary>
    int x;

    /// <summary>
    /// Y component
    /// </summary>
    int y;

    /// <summary>
    /// Z component
    /// </summary>
    int z;

    /// <summary>
    /// W component
    /// </summary>
    int w;
};

struct Vector4
{
public:
    /// <summary>
    /// Default constructor,
    /// sets 0 to all components of this vector.
    /// </summary>
    Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
    {
    }

    /// <summary>
    /// Sets given values to all components of this vector.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    /// <param name="w"></param>
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
    {
    }

public:
    /// <summary>
    /// Normalize this vector.
    /// </summary>
    FORCEINLINE void Normalize()
    {
        auto i = 1.0f / sqrt(x * x + y * y + z * z + w * w);

        x = x * i;
        y = y * i;
        z = z * i;
        w = w * i;
    }

    /// <summary>
    /// Calculate Length of this vector.
    /// </summary>
    /// <returns>The Length of this vector.</returns>
    FORCEINLINE float Length() const
    {
        return sqrt(x * x + y * y + z * z + w * w);
    }

public:
    /// <summary>
    /// Shorthand for [0, 0, 0, 0]
    /// </summary>
    FORCEINLINE static Vector4 Zero()
    {
        return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [1, 1, 1, 1]
    /// </summary>
    FORCEINLINE static Vector4 One()
    {
        return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    }

    /// <summary>
    /// Shorthand for [0, 0, 1, 0]
    /// </summary>
    FORCEINLINE static Vector4 Forward()
    {
        return Vector4(0.0f, 0.0f, 1.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [0, 0, -1, 0]
    /// </summary>
    FORCEINLINE static Vector4 Back()
    {
        return Vector4(0.0f, 0.0f, -1.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [1, 0, 0]
    /// </summary>
    FORCEINLINE static Vector4 Right()
    {
        return Vector4(1.0f, 0.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [-1, 0, 0, 0]
    /// </summary>
    FORCEINLINE static Vector4 Left()
    {
        return Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [0, 1, 0, 0]
    /// </summary>
    FORCEINLINE static Vector4 Up()
    {
        return Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [0, -1, 0, 0]
    /// </summary>
    FORCEINLINE static Vector4 Down()
    {
        return Vector4(0.0f, -1.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [0, 0, 0, 1]
    /// </summary>
    FORCEINLINE static Vector4 Deeper()
    {
        return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    }

    /// <summary>
    /// Shorthand for [0, 0, 0, -1]
    /// </summary>
    FORCEINLINE static Vector4 Closer()
    {
        return Vector4(0.0f, 0.0f, 0.0f, -1.0f);
    }

    /// <summary>
    /// Returns normalized vector
    /// </summary>
    FORCEINLINE static Vector4 Normalize(Vector4 vector)
    {
        vector.Normalize();
        return vector;
    }

    /// <summary>
    /// Returns normalized vector
    /// </summary>
    FORCEINLINE static void Normalize(Vector4 vector, Vector4* result)
    {
        *result = Normalize(vector);
    }

    /// <summary>
    /// Calculate Length of vector
    /// </summary>
    FORCEINLINE static float Length(Vector4 vector)
    {
        return vector.Length();
    }

    /// <summary>
    /// Calculate Length of vector
    /// </summary>
    FORCEINLINE static void Length(Vector4 vector, float* result)
    {
        *result = Length(vector);
    }

    /// <summary>
    /// Dot product of two vectors
    /// </summary>
    FORCEINLINE static float Dot(Vector4& a, Vector4& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }

    /// <summary>
    /// Dot product of two vectors
    /// </summary>
    FORCEINLINE static void Dot(Vector4& a, Vector4& b, float* result)
    {
        *result = Dot(a, b);
    }

    /// <summary>
    /// Linerally interpolates between two vectors
    /// </summary>
    FORCEINLINE static Vector4 Lerp(Vector4& a, Vector4& b, float t)
    {
        return Vector4(a.x * (1 - t) + t * b.x, a.y * (1 - t) + t * b.y, a.z * (1 - t) + t * b.z,
                       a.w * (1 - t) + t * b.w);
    }

    /// <summary>
    /// Linerally interpolates between two vectors
    /// </summary>
    FORCEINLINE static void Lerp(Vector4& a, Vector4& b, float t, Vector4* result)
    {
        *result = Lerp(a, b, t);
    }

    /// <summary>
    /// Negation of the vector
    /// </summary>
    FORCEINLINE static Vector4 Negate(Vector4& a)
    {
        return Vector4(a.x * -1.0f, a.y * -1.0f, a.z * -1.0f, a.w * -1.0f);
    }

    /// <summary>
    /// Negation of the vector
    /// </summary>
    FORCEINLINE static void Negate(Vector4& a, Vector4* result)
    {
        *result = Negate(a);
    }

    /// <summary>
    /// Absolute value of the vector
    /// </summary>
    FORCEINLINE static Vector4 Abs(Vector4& a)
    {
        return Vector4(fabs(a.x), fabs(a.y), fabs(a.z), fabs(a.w));
    }

    /// <summary>
    /// Absolute value of the vector
    /// </summary>
    FORCEINLINE static void Abs(Vector4& a, Vector4* result)
    {
        *result = Abs(a);
    }

    /// <summary>
    /// Distance between two vectors
    /// </summary>
    FORCEINLINE static float Distance(Vector4& a, Vector4& b)
    {
        return sqrtf(
            (b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z) + (b.w - a.w) * (b.w - a.w
            ));
    }

    /// <summary>
    /// Distance between two vectors
    /// </summary>
    FORCEINLINE static void Distance(Vector4& a, Vector4& b, float* result)
    {
        *result = Distance(a, b);
    }

public:
    void Vector4::operator -=(Vector4 r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;
        w -= r.w;
    }

    void operator +=(Vector4 r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
        w += r.w;
    }

    void Vector4::operator *=(float value)
    {
        x *= value;
        y *= value;
        z *= value;
        w *= value;
    }

    /// <summary>
    /// Result = Vector - Vector[r] operator
    /// </summary>
    Vector4 Vector4::operator -(Vector4 r) const
    {
        return Vector4(x - r.x, y - r.y, z - r.z, w - r.w);
    }

    /// <summary>
    /// Result = Vector + Vector[r] operator
    /// </summary>
    Vector4 Vector4::operator +(Vector4 r) const
    {
        return Vector4(x + r.x, y + r.y, z + r.z, w + r.w);
    }

public:
    /// <summary>
    /// X compoennt
    /// </summary>
    float x;

    /// <summary>
    /// Y component
    /// </summary>
    float y;

    /// <summary>
    /// Z component
    /// </summary>
    float z;

    /// <summary>
    /// W component
    /// </summary>
    float w;
};

#endif // Vector4_H
