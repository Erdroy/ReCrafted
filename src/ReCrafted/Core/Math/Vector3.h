#pragma once

#ifndef Vector3_H
#define Vector3_H

// includes
#include "../../recraftedPrerequisites.h"

struct Vector3
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this vector.
	/// </summary>
	Vector3() : X(0.0f), Y(0.0f), Z(0.0f) { }

	/// <summary>
	/// Sets given values to all components of this vector.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	Vector3(float x, float y, float z) : X(x), Y(y), Z(z) { }

public:

	void normalize()
	{
		// i = 1 / sqrt(v.x*v.x + v.y*v.y)
		// v = v*i

		auto i = 1 / sqrt(X*X + Y*Y + Z*Z);
		X = X * i;
		Y = Y * i;
		Z = Z * i;
	}

public:
	

	FORCEINLINE static Vector3 normalize(Vector3 vector)
	{
		vector.normalize();
		return vector;
	}

	FORCEINLINE static void normalize(Vector3 vector, Vector3* result)
	{
		*result = normalize(vector);
	}

	FORCEINLINE static float dot(Vector3 a, Vector3 b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
	}

	FORCEINLINE static void dot(Vector3 a, Vector3 b, float* result)
	{
		*result = dot(a, b);
	}

	FORCEINLINE static Vector3 cross(Vector3 a, Vector3 b)
	{
		return Vector3(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
	}

	FORCEINLINE static void cross(Vector3 a, Vector3 b, Vector3* result)
	{
		*result = cross(a, b);
	}

public:
	void operator -= (Vector3 r){
		X -= r.X;
		Y -= r.Y;
		Z -= r.Z;
	}

	/// <summary>
	/// Result = Vector - Vector[r] operator
	/// </summary>
	Vector3 operator - (Vector3 r)
	{
		return Vector3(X - r.X, Y - r.Y, Z - r.Z);
	}

public:
	/// <summary>
	/// X compoennt
	/// </summary>
	float X;

	/// <summary>
	/// Y component
	/// </summary>
	float Y;

	/// <summary>
	/// Z component
	/// </summary>
	float Z;
};

#endif // Vector3_H