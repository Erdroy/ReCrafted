// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef Rectf_H
#define Rectf_H

// includes
#include "ReCrafted.h"

ALIGN(4) struct Rectf
{
public:
    /// <summary>
    /// Default constructor
    /// Sets 0 to all components of this structure
    /// </summary>
    Rectf()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    Rectf(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
    {
    }

public:
    /// <summary>
    /// Size of this Rectangle
    /// </summary>
    FORCEINLINE Vector2 Size() const
    {
        return Vector2(width, height);
    }

    /// <summary>
    /// Size of this Rectangle
    /// </summary>
    FORCEINLINE void Size(const Vector2& size)
    {
        width = size.x;
        height = size.y;
    }

    FORCEINLINE Vector2 Center() const
    {
        return Vector2(x, y);
    }

    FORCEINLINE void Center(const Vector2& center)
    {
        x = center.x;
        y = center.y;
    }

    /// <summary>
    /// Minimum X coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float Left() const
    {
        return x - (width / 2);
    }

    /// <summary>
    /// Maximum X coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float Right() const
    {
        return x + (width / 2);
    }

    /// <summary>
    /// Minimum Y coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float Bottom() const
    {
        return y - (height / 2);
    }

    /// <summary>
    /// Maximum Y coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float Top() const
    {
        return y + (height / 2);
    }

public:
    /// <summary>
    /// Check if two Rectangles intersect each other
    /// </summary>
    FORCEINLINE static bool Intersects(Rectf& a, Rectf& b)
    {
        return (a.Left() < b.Right() && a.Right() > b.Left() && a.Top() < b.Bottom() && a.Bottom() > b.Top());
    }

    /// <summary>
    /// Check if two Rectangles intersect each other
    /// </summary>
    FORCEINLINE static void Intersects(Rectf& a, Rectf& b, bool* result)
    {
        *result = Intersects(a, b);
    }

    /// <summary>
    /// Check if point is within a Rectangle
    /// </summary>
    FORCEINLINE static bool Contains(Rectf& rect, Vector2& point)
    {
        return point.x > rect.Left() && point.x < rect.Right() && point.y > rect.Bottom() && point.y < rect.Top();
    }

    /// <summary>
    /// Check if point is within a Rectangle
    /// </summary>
    FORCEINLINE static void Contains(Rectf& rect, Vector2& point, bool* result)
    {
        *result = Contains(rect, point);
    }

public:
    /// <summary>
    /// X coordinate of this Rectangle
    /// </summary>
    float x;

    /// <summary>
    /// Y coordinate of this Rectangle
    /// </summary>
    float y;

    /// <summary>
    /// Width of this Rectangle
    /// </summary>
    float width;

    /// <summary>
    /// Height of this Rectangle
    /// </summary>
    float height;
};

#endif // Rectf_H
