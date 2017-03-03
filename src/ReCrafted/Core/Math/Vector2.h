// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef Vector2_H
#define Vector2_H

// includes
#include "../../recraftedPrerequisites.h"

struct Vector2
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this structure.
	/// </summary>
	Vector2()
	{
		X = 0.0f;
		Y = 0.0f;
	}

	Vector2(float x, float y) : X(x), Y(y)
	{
		
	}

public:
	/// <summary>
	/// Normalize this vector.
	/// </summary>
	FORCEINLINE void normalize()
	{
		auto i = 1.0f / sqrt(X*X + Y*Y);

		X = X * i;
		Y = Y * i;
	}

	/// <summary>
	/// Calculate length of this vector.
	/// </summary>
	/// <returns>The length of this vector.</returns>
	FORCEINLINE float length() const
	{
		return sqrt(X*X + Y*Y);
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
	FORCEINLINE static float length(Vector2 vector)
	{
		return vector.length();
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static void length(Vector2 vector, float* result)
	{
		*result = length(vector);
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static float dot(Vector2 a, Vector2 b)
	{
		return a.X * b.X + a.Y * b.Y;
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static void dot(Vector2 a, Vector2 b, float* result)
	{
		*result = dot(a, b);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static Vector2 lerp(Vector2 a, Vector2 b, float t)
	{
		return Vector2(a.X * (1 - t) + t * b.X, a.Y * (1 - t) + t * b.Y);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static void lerp(Vector2 a, Vector2 b, float t, Vector2* result)
	{
		*result = lerp(a, b, t);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static Vector2 negate(Vector2 a)
	{
		return Vector2(a.X * -1.0f, a.Y * -1.0f);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static void negate(Vector2 a, Vector2* result)
	{
		*result = negate(a);
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static Vector2 abs(Vector2 a)
	{
		return Vector2(fabs(a.X), fabs(a.Y));
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static void abs(Vector2 a, Vector2* result)
	{
		*result = abs(a);
	}

public:
	void Vector2::operator -= (Vector2 r) {
		X -= r.X;
		Y -= r.Y;
	}

	void Vector2::operator += (Vector2 r) {
		X += r.X;
		Y += r.Y;
	}

	Vector2 Vector2::operator - (Vector2 r) const
	{
		return Vector2(X - r.X, Y - r.Y);
	}

	Vector2 Vector2::operator + (Vector2 r) const
	{
		return Vector2(X + r.X, Y + r.Y);
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
};

#endif // Vector2_H