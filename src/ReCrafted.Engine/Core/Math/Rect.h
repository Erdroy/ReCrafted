// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef Rect_H
#define Rect_H

// includes
#include "ReCrafted.h"

struct Rect
{
public:
    /// <summary>
    /// Default constructor
    /// Sets 0 to all components of this structure
    /// </summary>
    Rect()
    {
        x = 0;
        y = 0;
        width = 0;
        height = 0;
    }

    Rect(int x, int y, int width, int height) : x(x), y(y), width(width), height(height)
    {
    }

public:
    /// <summary>
    /// Size of this Rectangle
    /// </summary>
    FORCEINLINE Vector2 Size() const
    {
        return Vector2(float(width), float(height));
    }

    /// <summary>
    /// Size of this Rectangle
    /// </summary>
    FORCEINLINE void Size(const Vector2& size)
    {
        width = static_cast<int>(size.x);
        height = static_cast<int>(size.y);
    }

    /// <summary>
    /// Center of this Rectangle
    /// </summary>
    FORCEINLINE Vector2 Center() const
    {
        return Vector2(float(x), float(y));
    }

    /// <summary>
    /// Center of this Rectangle
    /// </summary>
    FORCEINLINE void Center(const Vector2& center)
    {
        x = static_cast<int>(center.x);
        y = static_cast<int>(center.y);
    }

    /// <summary>
    /// Minimum X coordinate of this Rectangle
    /// </summary>
    FORCEINLINE int Left() const
    {
        return x - (width / 2);
    }

    /// <summary>
    /// Maximum X coordinate of this Rectangle
    /// </summary>
    FORCEINLINE int Right() const
    {
        return x + (width / 2);
    }

    /// <summary>
    /// Minimum Y coordinate of this Rectangle
    /// </summary>
    FORCEINLINE int Bottom() const
    {
        return y - (height / 2);
    }

    /// <summary>
    /// Maximum Y coordinate of this Rectangle
    /// </summary>
    FORCEINLINE int Top() const
    {
        return y + (height / 2);
    }

public:
    /// <summary>
    /// Check if two Rectangles intersect each other
    /// </summary>
    FORCEINLINE static bool Intersects(Rect& a, Rect& b)
    {
        return (a.Left() < b.Right() && a.Right() > b.Left() && a.Top() < b.Bottom() && a.Bottom() > b.Top());
    }

    /// <summary>
    /// Check if two Rectangles intersect each other
    /// </summary>
    FORCEINLINE static void Intersects(Rect& a, Rect& b, bool* result)
    {
        *result = Intersects(a, b);
    }

    /// <summary>
    /// Check if point is within a Rectangle
    /// </summary>
    FORCEINLINE static bool Contains(Rect& rect, Vector2& point)
    {
        return point.x > rect.Left() && point.x < rect.Right() && point.y > rect.Bottom() && point.y < rect.Top();
    }

    /// <summary>
    /// Check if point is within a Rectangle
    /// </summary>
    FORCEINLINE static void Contains(Rect& rect, Vector2& point, bool* result)
    {
        *result = Contains(rect, point);
    }

public:
    /// <summary>
    /// X coordinate of this Rectangle
    /// </summary>
    int x;

    /// <summary>
    /// Y coordinate of this Rectangle
    /// </summary>
    int y;

    /// <summary>
    /// Width of this Rectangle
    /// </summary>
    int width;

    /// <summary>
    /// Height of this Rectangle
    /// </summary>
    int height;
};

#endif // Rect_H
