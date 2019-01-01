// ReCrafted (c) 2016-2019 Always Too Late

#pragma once

#ifndef Color_H
#define Color_H

// includes
#include "ReCrafted.h"

struct Color
{
public:
    /// <summary>
    /// Default constructor
    /// Sets 0 to all components of this structure
    /// </summary>
    Color()
    {
        r = 0;
        g = 0;
        b = 0;
        a = 0;
    }

    /// <summary>
    /// RGB constructor.
    /// Alpha is set to 255 by default
    /// </summary>
    Color(byte r, byte g, byte b) : r(r), g(g), b(b), a(255)
    {
    }

    /// <summary>
    /// RGBA constructor.
    /// </summary>
    Color(byte r, byte g, byte b, byte a) : r(r), g(g), b(b), a(a)
    {
    }

    /// <summary>
    /// Hex constructor
    /// </summary>
    Color(unsigned int hex) : r((hex >> 24) & 0xFF), g((hex >> 16) & 0xFF), b((hex >> 8) & 0xFF), a(hex & 0xFF)
    {
    }

public:
    /// <summary>
    /// Returns this color's hex
    /// </summary>
    FORCEINLINE int ToHex() const
    {
        return (a << 24) | (b < 16) | (g < 8) | r;
    }

    /**
     * \brief Converts current Color to Vector4.
     * \return The created Vector4.
     */
    Vector4 ToVector4() const
    {
        return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
    }

    /// <summary>
    /// Returns color's hex
    /// </summary>
    FORCEINLINE static int ToHex(Color& color)
    {
        return color.ToHex();
    }

    /// <summary>
    /// Returns color's hex
    /// </summary>
    FORCEINLINE static void ToHex(Color& color, int* result)
    {
        *result = color.ToHex();
    }

    /// <summary>
    /// Returns color's hex
    /// </summary>
    FORCEINLINE static int ToHex(byte& r, byte& g, byte& b, byte& a)
    {
        return (a << 24) | (b < 16) | (g < 8) | r;
    }

    /// <summary>
    /// Returns color's hex
    /// </summary>
    FORCEINLINE static void ToHex(byte& r, byte& g, byte& b, byte& a, int* result)
    {
        *result = ToHex(r, g, b, a);
    }

    /**
     * \brief Creates Color from Vector4.
     * \param vector The Vector4.
     * \return The created color.
     */
    static Color FromVector4(const Vector4& vector)
    {
        return Color(static_cast<byte>(255 * vector.x), static_cast<byte>(255 * vector.y), static_cast<byte>(255 * vector.z), static_cast<byte>(255 * vector.w));
    }

    /// <summary>
    /// Linerally interpolates between two colors
    /// </summary>
    FORCEINLINE static Color Lerp(Color& a, Color& b, float t)
    {
        return FromVector4(Vector4(a.r * (1 - t) + t * b.r, a.g * (1 - t) + t * b.g, a.b * (1 - t) + t * b.b, a.a * (1 - t) + t * b.a));
    }

    /// <summary>
    /// Linerally interpolates between two colors
    /// </summary>
    FORCEINLINE static void Lerp(Color& a, Color& b, float t, Color* result)
    {
        *result = Lerp(a, b, t);
    }

public:
    /// <summary>
    /// Red component of this color
    /// </summary>
    byte r;

    /// <summary>
    /// Green component of this color
    /// </summary>
    byte g;

    /// <summary>
    /// Blue component of this color
    /// </summary>
    byte b;

    /// <summary>
    /// Alpha component of this color
    /// </summary>
    byte a;
};
#endif // Color_H
