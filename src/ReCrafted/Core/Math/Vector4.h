#pragma once

#ifndef Vector4_H
#define Vector4_H

// includes
#include "../../recraftedPrerequisites.h"

struct Vector4i
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this vector.
	/// </summary>
	Vector4i() : X(0), Y(0), Z(0), W(0) { }

	/// <summary>
	/// Sets given values to all components of this vector.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="w"></param>
	Vector4i(int x, int y, int z, int w) : X(x), Y(y), Z(z), W(w) { }

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

	/// <summary>
	/// W component
	/// </summary>
	int W;
};

struct Vector4
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this vector.
	/// </summary>
	Vector4() : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) { }

	/// <summary>
	/// Sets given values to all components of this vector.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	/// <param name="w"></param>
	Vector4(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }

public:
	/// <summary>
	/// Normalize this vector.
	/// </summary>
	FORCEINLINE void normalize()
	{
		auto i = 1.0f / sqrt(X*X + Y*Y + Z*Z + W*W);

		X = X * i;
		Y = Y * i;
		Z = Z * i;
		W = W * i;
	}

	/// <summary>
	/// Calculate length of this vector.
	/// </summary>
	/// <returns>The length of this vector.</returns>
	FORCEINLINE float length() const
	{
		return sqrt(X*X + Y*Y + Z*Z + W*W);
	}

public:
	/// <summary>
	/// Shorthand for [0, 0, 0, 0]
	/// </summary>
	FORCEINLINE static Vector4 zero()
	{
		return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [1, 1, 1, 1]
	/// </summary>
	FORCEINLINE static Vector4 one()
	{
		return Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	/// <summary>
	/// Shorthand for [0, 0, 1, 0]
	/// </summary>
	FORCEINLINE static Vector4 forward()
	{
		return Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, 0, -1, 0]
	/// </summary>
	FORCEINLINE static Vector4 back()
	{
		return Vector4(0.0f, 0.0f, -1.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [1, 0, 0]
	/// </summary>
	FORCEINLINE static Vector4 right()
	{
		return Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [-1, 0, 0, 0]
	/// </summary>
	FORCEINLINE static Vector4 left()
	{
		return Vector4(-1.0f, 0.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, 1, 0, 0]
	/// </summary>
	FORCEINLINE static Vector4 up()
	{
		return Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, -1, 0, 0]
	/// </summary>
	FORCEINLINE static Vector4 down()
	{
		return Vector4(0.0f, -1.0f, 0.0f, 0.0f);
	}

	/// <summary>
	/// Shorthand for [0, 0, 0, 1]
	/// </summary>
	FORCEINLINE static Vector4 deeper()
	{
		return Vector4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	/// <summary>
	/// Shorthand for [0, 0, 0, -1]
	/// </summary>
	FORCEINLINE static Vector4 closer()
	{
		return Vector4(0.0f, 0.0f, 0.0f, -1.0f);
	}

	/// <summary>
	/// Returns normalized vector
	/// </summary>
	FORCEINLINE static Vector4 normalize(Vector4 vector)
	{
		vector.normalize();
		return vector;
	}

	/// <summary>
	/// Returns normalized vector
	/// </summary>
	FORCEINLINE static void normalize(Vector4 vector, Vector4* result)
	{
		*result = normalize(vector);
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static float length(Vector4 vector)
	{
		return vector.length();
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static void length(Vector4 vector, float* result)
	{
		*result = length(vector);
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static float dot(Vector4 a, Vector4 b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static void dot(Vector4 a, Vector4 b, float* result)
	{
		*result = dot(a, b);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static Vector4 lerp(Vector4 a, Vector4 b, float t)
	{
		return Vector4(a.X * (1 - t) + t * b.X, a.Y * (1 - t) + t * b.Y, a.Z * (1 - t) + t * b.Z, a.W * (1 - t) + t * b.W);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static void lerp(Vector4 a, Vector4 b, float t, Vector4* result)
	{
		*result = lerp(a, b, t);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static Vector4 negate(Vector4 a)
	{
		return Vector4(a.X * -1.0f, a.Y * -1.0f, a.Z * -1.0f, a.W * -1.0f);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static void negate(Vector4 a, Vector4* result)
	{
		*result = negate(a);
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static Vector4 abs(Vector4 a)
	{
		return Vector4(fabs(a.X), fabs(a.Y), fabs(a.Z), fabs(a.W));
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static void abs(Vector4 a, Vector4* result)
	{
		*result = abs(a);
	}

public:
	void Vector4::operator -= (Vector4 r) {
		X -= r.X;
		Y -= r.Y;
		Z -= r.Z;
		W -= r.W;
	}

	void operator += (Vector4 r) {
		X += r.X;
		Y += r.Y;
		Z += r.Z;
		W += r.W;
	}

	void Vector4::operator *= (float value) {
		X *= value;
		Y *= value;
		Z *= value;
		W *= value;
	}

	/// <summary>
	/// Result = Vector - Vector[r] operator
	/// </summary>
	Vector4 Vector4::operator - (Vector4 r) const
	{
		return Vector4(X - r.X, Y - r.Y, Z - r.Z, W - r.W);
	}

	/// <summary>
	/// Result = Vector + Vector[r] operator
	/// </summary>
	Vector4 Vector4::operator + (Vector4 r) const
	{
		return Vector4(X + r.X, Y + r.Y, Z + r.Z, W + r.W);
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

	/// <summary>
	/// W component
	/// </summary>
	float W;
};

#endif // Vector4_H
