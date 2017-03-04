// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef PLANE_H
#define PLANE_H

// includes
#include "Vector3.h"
#include "Math.h"

struct Plane
{
private:

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
		normal.X = normal.Y = normal.Z = distance = value;
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
		normal.X = a;
		normal.Y = b;
		normal.Z = c;
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
	/// <param name="point1">First point of a triangle defining the plane.</param>
	/// <param name="point2">Second point of a triangle defining the plane.</param>
	/// <param name="point3">Third point of a triangle defining the plane.</param>
	explicit Plane(Vector3 point1, Vector3 point2, Vector3 point3)
	{
		auto x1 = point2.X - point1.X;
		auto y1 = point2.Y - point1.Y;
		auto z1 = point2.Z - point1.Z;
		auto x2 = point3.X - point1.X;
		auto y2 = point3.Y - point1.Y;
		auto z2 = point3.Z - point1.Z;
		auto yz = (y1 * z2) - (z1 * y2);
		auto xz = (z1 * x2) - (x1 * z2);
		auto xy = (x1 * y2) - (y1 * x2);
		auto invPyth = 1.0f / static_cast<float>(Math::sqrtf((yz * yz) + (xz * xz) + (xy * xy)));

		normal.X = yz * invPyth;
		normal.Y = xz * invPyth;
		normal.Z = xy * invPyth;
		distance = -((normal.X * point1.X) + (normal.Y * point1.Y) + (normal.Z * point1.Z));
	}

public:
	/// <summary>
	/// Changes the coefficients of the normal vector of the plane to make it of unit length.
	/// </summary>
	FORCEINLINE void Normalize()
	{
		auto magnitude = 1.0f / static_cast<float>(Math::sqrtf((normal.X * normal.X) + (normal.Y * normal.Y) + (normal.Z * normal.Z)));

		normal.X *= magnitude;
		normal.Y *= magnitude;
		normal.Z *= magnitude;
		distance *= magnitude;
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
