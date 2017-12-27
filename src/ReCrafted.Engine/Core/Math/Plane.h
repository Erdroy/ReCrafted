// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef PLANE_H
#define PLANE_H

// includes
#include "Vector3.h"
#include "Math.h"

enum PlaneIntersection
{
	None,
	Intersecting,
	Back,
	Front
};

struct Plane
{
public:
	/// <summary>
	/// Initializes a new instance of the Plane struct.
	/// </summary>
	Plane() { }

	/// <summary>
	/// Initializes a new instance of the Plane struct.
	/// </summary>
	/// <param name="value">The value that will be assigned to all components.</param>
	explicit Plane(float value)
	{
		normal.x = normal.y = normal.z = distance = value;
	}

	/// <summary>
	/// Initializes a new instance of the Plane struct.
	/// </summary>
	/// <param name="a">The X component of the normal.</param>
	/// <param name="b">The Y component of the normal.</param>
	/// <param name="c">The Z component of the normal.</param>
	/// <param name="d">The distance of the plane along its normal from the origin.</param>
	explicit Plane(float a, float b, float c, float d)
	{
		normal.x = a;
		normal.y = b;
		normal.z = c;
		distance = d;
	}

	/// <summary>
	/// Initializes a new instance of the Plane class.
	/// </summary>
	/// <param name="point">Any point that lies along the plane.</param>
	/// <param name="normal">The normal vector to the plane.</param>
	explicit Plane(Vector3 point, Vector3 normal)
	{
		normal = normal;
		distance = -Vector3::dot(normal, point);
	}

	/// <summary>
	/// Initializes a new instance of the Plane struct.
	/// </summary>
	/// <param name="a">First point of a triangle defining the plane.</param>
	/// <param name="b">Second point of a triangle defining the plane.</param>
	/// <param name="c">Third point of a triangle defining the plane.</param>
	explicit Plane(Vector3 a, Vector3 b, Vector3 c)
	{
		var xa = b - a;
		var xb = a - c;
		var dir = Vector3::cross(xa, xb);
		dir.normalize();
		normal = dir;
		distance = dir.length();
	}

public:
    FORCEINLINE float dot(const Vector3 point) const
    {
        return normal.x * point.x 
        + normal.y * point.y 
        + normal.z * point.z 
        + distance * 1;
    }

public:
	/// <summary>
	/// Changes the coefficients of the normal vector of the plane to make it of unit length.
	/// </summary>
	FORCEINLINE void normalize()
	{
		auto magnitude = 1.0f / static_cast<float>(Math::sqrtf((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z)));

		normal.x *= magnitude;
		normal.y *= magnitude;
		normal.z *= magnitude;
		distance = magnitude;
	}
public:
	/// <summary>
	/// Determines whether there is an intersection between a <see cref="Plane"/> and a point.
	/// </summary>
	/// <param name="plane">The plane to test.</param>
	/// <param name="point">The point to test.</param>
	/// <returns>Whether the two objects intersected.</returns>
	FORCEINLINE static PlaneIntersection planeIntersectsPoint(Plane& plane, Vector3& point)
	{
		float distance;
		Vector3::dot(plane.normal, point, &distance);
		distance += plane.distance;

		if (distance > 0.0f)
			return PlaneIntersection::Front;

		if (distance < 0.0f)
			return PlaneIntersection::Back;

		return PlaneIntersection::Intersecting;
	}

public:
	/// <summary>
	/// The normal vector of the plane.
	/// </summary>
	Vector3 normal = {};

	/// <summary>
	/// The distance of the plane along its normal from the origin.
	/// </summary>
	float distance = 0.0f;
};

#endif // PLANE_H
