// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef Rectf_H
#define Rectf_H

// includes
#include "ReCrafted.h"
#include "Vector2.h"

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
    FORCEINLINE Vector2 size() const
    {
        return Vector2(width, height);
    }

    /// <summary>
    /// Size of this Rectangle
    /// </summary>
    FORCEINLINE void size(const Vector2& size)
    {
        width = size.x;
        height = size.y;
    }

    FORCEINLINE Vector2 center() const
    {
        return Vector2(x, y);
    }

    FORCEINLINE void center(const Vector2& center)
    {
        x = center.x;
        y = center.y;
    }

    /// <summary>
    /// Minimum X coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float left() const
    {
        return x - (width / 2);
    }

    /// <summary>
    /// Maximum X coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float right() const
    {
        return x + (width / 2);
    }

    /// <summary>
    /// Minimum Y coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float bottom() const
    {
        return y - (height / 2);
    }

    /// <summary>
    /// Maximum Y coordinate of this Rectangle
    /// </summary>
    FORCEINLINE float top() const
    {
        return y + (height / 2);
    }

public:
    /// <summary>
    /// Check if two Rectangles intersect each other
    /// </summary>
    FORCEINLINE static bool intersects(Rectf& a, Rectf& b)
    {
        return (a.left() < b.right() && a.right() > b.left() && a.top() < b.bottom() && a.bottom() > b.top());
    }

    /// <summary>
    /// Check if two Rectangles intersect each other
    /// </summary>
    FORCEINLINE static void intersects(Rectf& a, Rectf& b, bool* result)
    {
        *result = intersects(a, b);
    }

    /// <summary>
    /// Check if point is within a Rectangle
    /// </summary>
    FORCEINLINE static bool contains(Rectf& rect, Vector2& point)
    {
        return point.x > rect.left() && point.x < rect.right() && point.y > rect.bottom() && point.y < rect.top();
    }

    /// <summary>
    /// Check if point is within a Rectangle
    /// </summary>
    FORCEINLINE static void contains(Rectf& rect, Vector2& point, bool* result)
    {
        *result = contains(rect, point);
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
