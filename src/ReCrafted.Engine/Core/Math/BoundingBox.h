// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef BoundingBox_H
#define BoundingBox_H

// includes
#include "ReCrafted.h"
#include "Vector3.h"

struct BoundingBox
{
public:
    /// <summary>
    /// Default constructor
    /// Sets 0 to all components of this structure
    /// </summary>
    BoundingBox()
    {
        center = Vector3::Zero();
        size = Vector3::Zero();
    }

    /// <summary>
    /// Constructs BoundingBox with given center and size.
    /// </summary>
    /// <param name="center">The center of the BoundingBox.</param>
    /// <param name="size">The size of the BoundingBox.</param>
    explicit BoundingBox(Vector3& center, Vector3& size)
    {
        this->center = center;
        this->size = size;
    }

public:
    /// <summary>
    /// Minimum X coordinate of this BoundingBox
    /// </summary>
    FORCEINLINE float Left() const
    {
        return center.x - (size.x / 2);
    }

    /// <summary>
    /// Maximum X coordinate of this BoundingBox
    /// </summary>
    FORCEINLINE float Right() const
    {
        return center.x + (size.x / 2);
    }

    /// <summary>
    /// Minimum Y coordinate of this BoundingBox
    /// </summary>
    FORCEINLINE float Bottom() const
    {
        return center.y - (size.y / 2);
    }

    /// <summary>
    /// Maximum Y coordinate of this BoundingBox
    /// </summary>
    FORCEINLINE float Top() const
    {
        return center.y + (size.y / 2);
    }

    /// <summary>
    /// Minimum Z coordinate of this BoundingBox
    /// </summary>
    FORCEINLINE float Back() const
    {
        return center.z - (size.z / 2);
    }

    /// <summary>
    /// Maximum Z coordinate of this BoundingBox
    /// </summary>
    FORCEINLINE float Front() const
    {
        return center.z + (size.z / 2);
    }

    /// <summary>
    /// The maximum of the bounding box.
    /// </summary>
    FORCEINLINE Vector3 Maximum() const
    {
        return center + size * 0.5f;
    }

    /// <summary>
    /// The minimum of the bounding box.
    /// </summary>
    FORCEINLINE Vector3 Minimum() const
    {
        return center - size * 0.5f;
    }

    /// <summary>
    /// Check if two BoundingBoxes intersect each other
    /// </summary>
    FORCEINLINE static bool Intersects(BoundingBox& a, BoundingBox& b)
    {
        cvar amin = a.Minimum();
        cvar bmin = b.Minimum();
        cvar amax = a.Maximum();
        cvar bmax = b.Maximum();

        if (amin.x > bmax.x || bmin.x > amax.x)
            return false;

        if (amin.y > bmax.y || bmin.y > amax.y)
            return false;

        if (amin.z > bmax.z || bmin.z > amax.z)
            return false;

        return true;
    }

    /// <summary>
    /// Check if two BoundingBoxes intersect each other
    /// </summary>
    FORCEINLINE static void Intersects(BoundingBox& a, BoundingBox& b, bool* result)
    {
        *result = Intersects(a, b);
    }

    /// <summary>
    /// Check if point is within a BoundingBox
    /// </summary>
    FORCEINLINE static bool Contains(BoundingBox& box, Vector3& point)
    {
        return point.x > box.Left() && point.x < box.Right() && point.y > box.Bottom() && point.y < box.Top() && point.z
            > box.Back() && point.z < box.Front();
    }

    /// <summary>
    /// Check if point is within a BoundingBox
    /// </summary>
    FORCEINLINE static void Contains(BoundingBox& box, Vector3& point, bool* result)
    {
        *result = Contains(box, point);
    }

public:
    /// <summary>
    /// Center of this BoundingBox
    /// </summary>
    Vector3 center;

    /// <summary>
    /// Size of this BoundingBox
    /// </summary>
    Vector3 size;
};

#endif // Rectf_H
