#pragma once

#ifndef Vector3_H
#define Vector3_H

// includes
#include "../../recraftedPrerequisites.h"

struct Vector3i
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this vector.
	/// </summary>
	Vector3i() : X(0), Y(0), Z(0) { }

	/// <summary>
	/// Sets given values to all components of this vector.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	Vector3i(int x, int y, int z) : X(x), Y(y), Z(z) { }

public:
	/// <summary>
	/// X compoennt
	/// </summary>
	int X;

	/// <summary>
	/// Y component
	/// </summary>
	int Y;

	/// <summary>
	/// Z component
	/// </summary>
	int Z;
};

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
	/// <summary>
	/// Normalize this vector.
	/// </summary>
	FORCEINLINE void normalize()
	{
		auto i = 1.0f / sqrt(X*X + Y*Y + Z*Z);

		X = X * i;
		Y = Y * i;
		Z = Z * i;
	}

	/// <summary>
	/// Calculate length of this vector.
	/// </summary>
	/// <returns>The length of this vector.</returns>
	FORCEINLINE float length() const
	{
		return sqrt(X*X + Y*Y + Z*Z);
	}

public:
	/// <summary>
	/// Shorthand for [0, 0, 0]
	/// </summary>
	FORCEINLINE static Vector3 zero()
	{
		return Vector3(0.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [1, 1, 1]
	/// </summary>
	FORCEINLINE static Vector3 one()
	{
		return Vector3(1.0f, 1.0f, 1.0f);
	}

	/// <summary>
	/// Shorthand for [0, 0, 1]
	/// </summary>
	FORCEINLINE static Vector3 forward() 
	{
		return Vector3(0.0f, 0.0f, 1.0f);
	}

	/// <summary>
	/// Shorthand for [0, 0, -1]
	/// </summary>
	FORCEINLINE static Vector3 back()
	{
		return Vector3(0.0f, 0.0f, -1.0f);
	}

	/// <summary>
	/// Shorthand for [1, 0, 0]
	/// </summary>
	FORCEINLINE static Vector3 right()
	{
		return Vector3(1.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [-1, 0, 0]
	/// </summary>
	FORCEINLINE static Vector3 left()
	{
		return Vector3(-1.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, 1, 0]
	/// </summary>
	FORCEINLINE static Vector3 up()
	{
		return Vector3(0.0f, 1.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, -1, 0]
	/// </summary>
	FORCEINLINE static Vector3 down()
	{
		return Vector3(0.0f, -1.0f, 0.0f);
	}

	FORCEINLINE static Vector3 normalize(Vector3 vector)
	{
		vector.normalize();
		return vector;
	}

	FORCEINLINE static void normalize(Vector3 vector, Vector3* result)
	{
		*result = normalize(vector);
	}

	FORCEINLINE static float length(Vector3 vector)
	{
		return vector.length();
	}

	FORCEINLINE static void length(Vector3 vector, float* result)
	{
		*result = length(vector);
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

	FORCEINLINE static Vector3 lerp(Vector3 a, Vector3 b, float t) 
	{
		return Vector3(a.X * (1 - t) + t * b.X, a.Y * (1 - t) + t * b.Y, a.Z * (1 - t) + t * b.Z);
	}

	FORCEINLINE static void lerp(Vector3 a, Vector3 b, float t, Vector3* result)
	{
		*result = lerp(a, b, t);
	}
public:
	void Vector3::operator -= (Vector3 r){
		X -= r.X;
		Y -= r.Y;
		Z -= r.Z;
	}

	void operator += (Vector3 r) {
		X += r.X;
		Y += r.Y;
		Z += r.Z;
	}

	void Vector3::operator *= (float value) {
		X *= value;
		Y *= value;
		Z *= value;
	}

	/// <summary>
	/// Result = Vector - Vector[r] operator
	/// </summary>
	Vector3 Vector3::operator - (Vector3 r) const
	{
		return Vector3(X - r.X, Y - r.Y, Z - r.Z);
	}

	/// <summary>
	/// Result = Vector + Vector[r] operator
	/// </summary>
	Vector3 Vector3::operator + (Vector3 r) const
	{
		return Vector3(X + r.X, Y + r.Y, Z + r.Z);
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