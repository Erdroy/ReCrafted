// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef BoundingBox_H
#define BoundingBox_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Core/Math/Vector3.h"

struct BoundingBox
{
public:
	/// <summary>
	/// Default constructor
	/// Sets 0 to all components of this structure
	/// </summary>
	BoundingBox()
	{
		center = Vector3::zero();
		size = Vector3::zero();
	}

public:
	/// <summary>
	/// Minimum X coordinate of this BoundingBox
	/// </summary>
	FORCEINLINE float left() const
	{
		return center.x - (size.x / 2);
	}

	/// <summary>
	/// Maximum X coordinate of this BoundingBox
	/// </summary>
	FORCEINLINE float right() const
	{
		return center.x + (size.x / 2);
	}

	/// <summary>
	/// Minimum Y coordinate of this BoundingBox
	/// </summary>
	FORCEINLINE float bottom() const
	{
		return center.y - (size.y / 2);
	}

	/// <summary>
	/// Maximum Y coordinate of this BoundingBox
	/// </summary>
	FORCEINLINE float top() const
	{
		return center.y + (size.y / 2);
	}

	/// <summary>
	/// Minimum Z coordinate of this BoundingBox
	/// </summary>
	FORCEINLINE float back() const 
	{
		return center.z - (size.z / 2);
	}

	/// <summary>
	/// Maximum Z coordinate of this BoundingBox
	/// </summary>
	FORCEINLINE float front() const 
	{
		return center.z + (size.z / 2);
	}

	/// <summary>
	/// Check if two BoundingBoxes intersect each other
	/// </summary>
	FORCEINLINE static bool intersects(BoundingBox& a, BoundingBox& b) 
	{
		return (a.left < b.right && a.right > b.left && a.top < b.bottom && a.bottom > b.top && a.back < b.front && a.front > b.back);
	}

	/// <summary>
	/// Check if two BoundingBoxes intersect each other
	/// </summary>
	FORCEINLINE static void intersects(BoundingBox& a, BoundingBox& b, bool* result)
	{
		*result = intersects(a, b);
	}

	/// <summary>
	/// Check if point is within a BoundingBox
	/// </summary>
	FORCEINLINE static bool contains(BoundingBox& box, Vector3& point)
	{
		return point.x > box.left && point.x < box.right && point.y > box.bottom && point.y < box.top && point.z > box.back && point.z < box.front;
	}

	/// <summary>
	/// Check if point is within a BoundingBox
	/// </summary>
	FORCEINLINE static void contains(BoundingBox& box, Vector3& point, bool* result)
	{
		*result = contains(box, point);
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