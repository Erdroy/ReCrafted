// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef Vector3_H
#define Vector3_H

// includes
#include "ReCrafted.h"

struct Vector3
{
public:
    /// <summary>
    /// Default constructor,
    /// sets 0 to all components of this vector.
    /// </summary>
    Vector3() : x(0.0f), y(0.0f), z(0.0f)
    {
    }

    /// <summary>
    /// Sets given values to all components of this vector.
    /// </summary>
    /// <param name="x"></param>
    /// <param name="y"></param>
    /// <param name="z"></param>
    Vector3(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

public:
    /// <summary>
    /// Normalize this vector.
    /// </summary>
    FORCEINLINE void Normalize()
    {
        auto i = 1.0f / sqrt(x * x + y * y + z * z);

        x = x * i;
        y = y * i;
        z = z * i;
    }

    /// <summary>
    /// Calculate Length of this vector.
    /// </summary>
    /// <returns>The Length of this vector.</returns>
    FORCEINLINE float Length() const
    {
        return sqrt(x * x + y * y + z * z);
    }

    /// <summary>
    /// Calculate squared Length of this vector.
    /// </summary>
    /// <returns>The Length of this vector.</returns>
    FORCEINLINE float LengthSqr() const
    {
        return x * x + y * y + z * z;
    }

public:
    /// <summary>
    /// Shorthand for [0, 0, 0]
    /// </summary>
    FORCEINLINE static Vector3 Zero()
    {
        return Vector3(0.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [1, 1, 1]
    /// </summary>
    FORCEINLINE static Vector3 One()
    {
        return Vector3(1.0f, 1.0f, 1.0f);
    }

    /// <summary>
    /// Shorthand for [0, 0, 1]
    /// </summary>
    FORCEINLINE static Vector3 Forward()
    {
        return Vector3(0.0f, 0.0f, 1.0f);
    }

    /// <summary>
    /// Shorthand for [0, 0, -1]
    /// </summary>
    FORCEINLINE static Vector3 Back()
    {
        return Vector3(0.0f, 0.0f, -1.0f);
    }

    /// <summary>
    /// Shorthand for [1, 0, 0]
    /// </summary>
    FORCEINLINE static Vector3 Right()
    {
        return Vector3(1.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [-1, 0, 0]
    /// </summary>
    FORCEINLINE static Vector3 Left()
    {
        return Vector3(-1.0f, 0.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [0, 1, 0]
    /// </summary>
    FORCEINLINE static Vector3 Up()
    {
        return Vector3(0.0f, 1.0f, 0.0f);
    }

    /// <summary>
    /// Shorthand for [0, -1, 0]
    /// </summary>
    FORCEINLINE static Vector3 Down()
    {
        return Vector3(0.0f, -1.0f, 0.0f);
    }

    /// <summary>
    /// Returns normalized vector
    /// </summary>
    FORCEINLINE static Vector3 Normalize(Vector3 vector)
    {
        vector.Normalize();
        return vector;
    }

    /// <summary>
    /// Returns normalized vector
    /// </summary>
    FORCEINLINE static void Normalize(Vector3 vector, Vector3* result)
    {
        *result = Normalize(vector);
    }

    /// <summary>
    /// Calculate Length of vector
    /// </summary>
    FORCEINLINE static float Length(Vector3 vector)
    {
        return vector.Length();
    }

    /// <summary>
    /// Calculate Length of vector
    /// </summary>
    FORCEINLINE static void Length(Vector3 vector, float* result)
    {
        *result = Length(vector);
    }

    /// <summary>
    /// Dot product of two vectors
    /// </summary>
    FORCEINLINE static float Dot(Vector3& a, Vector3& b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    /// <summary>
    /// Dot product of two vectors
    /// </summary>
    FORCEINLINE static void Dot(Vector3& a, Vector3& b, float* result)
    {
        *result = Dot(a, b);
    }

    /// <summary>
    /// Cross product of two vectors
    /// </summary>
    FORCEINLINE static Vector3 Cross(Vector3& a, Vector3& b)
    {
        return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
    }

    /// <summary>
    /// Cross product of two vectors
    /// </summary>
    FORCEINLINE static void Cross(Vector3& a, Vector3& b, Vector3* result)
    {
        *result = Cross(a, b);
    }

    /// <summary>
    /// Linerally interpolates between two vectors
    /// </summary>
    FORCEINLINE static Vector3 Lerp(Vector3& a, Vector3& b, float t)
    {
        return Vector3(a.x * (1 - t) + t * b.x, a.y * (1 - t) + t * b.y, a.z * (1 - t) + t * b.z);
    }

    /// <summary>
    /// Linerally interpolates between two vectors
    /// </summary>
    FORCEINLINE static void Lerp(Vector3& a, Vector3& b, float t, Vector3* result)
    {
        *result = Lerp(a, b, t);
    }

    /// <summary>
    /// Negation of the vector
    /// </summary>
    FORCEINLINE static Vector3 Negate(Vector3& a)
    {
        return Vector3(a.x * -1.0f, a.y * -1.0f, a.z * -1.0f);
    }

    /// <summary>
    /// Negation of the vector
    /// </summary>
    FORCEINLINE static void Negate(Vector3& a, Vector3* result)
    {
        *result = Negate(a);
    }

    /// <summary>
    /// Absolute value of the vector
    /// </summary>
    FORCEINLINE static Vector3 Abs(Vector3& a)
    {
        return Vector3(fabs(a.x), fabs(a.y), fabs(a.z));
    }

    /// <summary>
    /// Absolute value of the vector
    /// </summary>
    FORCEINLINE static void Abs(Vector3& a, Vector3* result)
    {
        *result = Abs(a);
    }

    /// <summary>
    /// Distance between two vectors
    /// </summary>
    FORCEINLINE static float Distance(Vector3& a, Vector3& b)
    {
        return sqrtf((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y) + (b.z - a.z) * (b.z - a.z));
    }

    /// <summary>
    /// Distance between two vectors
    /// </summary>
    FORCEINLINE static void Distance(Vector3& a, Vector3& b, float* result)
    {
        *result = Distance(a, b);
    }

public:
    void Vector3::operator -=(Vector3 r)
    {
        x -= r.x;
        y -= r.y;
        z -= r.z;
    }

    void operator +=(Vector3 r)
    {
        x += r.x;
        y += r.y;
        z += r.z;
    }

    void Vector3::operator *=(float value)
    {
        x *= value;
        y *= value;
        z *= value;
    }

    /// <summary>
    /// Result = Vector - Vector[r] operator
    /// </summary>
    Vector3 Vector3::operator -(Vector3 r) const
    {
        return Vector3(x - r.x, y - r.y, z - r.z);
    }

    /// <summary>
    /// Result = Vector + Vector[r] operator
    /// </summary>
    Vector3 Vector3::operator +(Vector3 r) const
    {
        return Vector3(x + r.x, y + r.y, z + r.z);
    }

    /// <summary>
    /// Result = Vector * Vector[r] operator
    /// </summary>
    Vector3 Vector3::operator *(Vector3 r) const
    {
        return Vector3(x * r.x, y * r.y, z * r.z);
    }

    /// <summary>
    /// Result = Vector * float operator
    /// </summary>
    Vector3 Vector3::operator *(float r) const
    {
        return Vector3(x * r, y * r, z * r);
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
};

#endif // Vector3_H
