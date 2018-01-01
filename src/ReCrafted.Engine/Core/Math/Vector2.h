// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef Vector2_H
#define Vector2_H

// includes
#include "ReCrafted.h"

struct Vector2
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this structure.
	/// </summary>
	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float x, float y) : x(x), y(y)
	{
		
	}

public:
	/// <summary>
	/// Normalize this vector.
	/// </summary>
	FORCEINLINE void normalize()
	{
		auto i = 1.0f / sqrt(x*x + y*y);

		x = x * i;
		y = y * i;
	}

	/// <summary>
	/// Calculate length of this vector.
	/// </summary>
	/// <returns>The length of this vector.</returns>
	FORCEINLINE float length() const
	{
		return sqrt(x*x + y*y);
	}

public:
public:
	/// <summary>
	/// Shorthand for [0, 0]
	/// </summary>
	FORCEINLINE static Vector2 zero()
	{
		return Vector2(0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [1, 1]
	/// </summary>
	FORCEINLINE static Vector2 one()
	{
		return Vector2(1.0f, 1.0f);
	}

	/// <summary>
	/// Shorthand for [1, 0]
	/// </summary>
	FORCEINLINE static Vector2 right()
	{
		return Vector2(1.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [-1, 0]
	/// </summary>
	FORCEINLINE static Vector2 left()
	{
		return Vector2(-1.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, 1]
	/// </summary>
	FORCEINLINE static Vector2 up()
	{
		return Vector2(0.0f, 1.0f);
	}

	/// <summary>
	/// Shorthand for [0, -1]
	/// </summary>
	FORCEINLINE static Vector2 down()
	{
		return Vector2(0.0f, -1.0f);
	}

	/// <summary>
	/// Returns normalized vector
	/// </summary>
	FORCEINLINE static Vector2 normalize(Vector2 vector)
	{
		vector.normalize();
		return vector;
	}

	/// <summary>
	/// Returns normalized vector
	/// </summary>
	FORCEINLINE static void normalize(Vector2 vector, Vector2* result)
	{
		*result = normalize(vector);
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static float length(Vector2& vector)
	{
		return vector.length();
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static void length(Vector2& vector, float* result)
	{
		*result = length(vector);
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static float dot(Vector2& a, Vector2& b)
	{
		return a.x * b.x + a.y * b.y;
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static void dot(Vector2& a, Vector2& b, float* result)
	{
		*result = dot(a, b);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static Vector2 lerp(Vector2& a, Vector2& b, float t)
	{
		return Vector2(a.x * (1 - t) + t * b.x, a.y * (1 - t) + t * b.y);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static void lerp(Vector2& a, Vector2& b, float t, Vector2* result)
	{
		*result = lerp(a, b, t);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static Vector2 negate(Vector2& a)
	{
		return Vector2(a.x * -1.0f, a.y * -1.0f);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static void negate(Vector2& a, Vector2* result)
	{
		*result = negate(a);
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static Vector2 abs(Vector2& a)
	{
		return Vector2(fabs(a.x), fabs(a.y));
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static void abs(Vector2& a, Vector2* result)
	{
		*result = abs(a);
	}

	/// <summary>
	/// Distance between two vectors
	/// </summary>
	FORCEINLINE static float distance(Vector2& a, Vector2& b) 
	{
		return sqrtf((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y));
	}

	/// <summary>
	/// Distance between two vectors
	/// </summary>
	FORCEINLINE static void distance(Vector2& a, Vector2& b, float* result) 
	{
		*result = distance(a, b);
	}

public:
	void Vector2::operator -= (Vector2 r) {
		x -= r.x;
		y -= r.y;
	}

	void Vector2::operator += (Vector2 r) {
		x += r.x;
		y += r.y;
	}

	Vector2 Vector2::operator - (Vector2 r) const
	{
		return Vector2(x - r.x, y - r.y);
	}

	Vector2 Vector2::operator + (Vector2 r) const
	{
		return Vector2(x + r.x, y + r.y);
	}

public:
	/// <summary>
	/// X component
	/// </summary>
	float x;

	/// <summary>
	/// Y component
	/// </summary>
	float y;
};

#endif // Vector2_H