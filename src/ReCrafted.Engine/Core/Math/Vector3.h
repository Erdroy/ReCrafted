// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef Vector3_H
#define Vector3_H

// includes
#include "ReCrafted.h"

struct Vector3
{
public:
	/// <summary>
	/// Default constructor,
	/// sets 0 to all components of this vector.
	/// </summary>
	Vector3() : x(0.0f), y(0.0f), z(0.0f) { }

	/// <summary>
	/// Sets given values to all components of this vector.
	/// </summary>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="z"></param>
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { }

public:
	/// <summary>
	/// Normalize this vector.
	/// </summary>
	FORCEINLINE void normalize()
	{
		auto i = 1.0f / sqrt(x*x + y*y + z*z);

		x = x * i;
		y = y * i;
		z = z * i;
	}

	/// <summary>
	/// Calculate length of this vector.
	/// </summary>
	/// <returns>The length of this vector.</returns>
	FORCEINLINE float length() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	/// <summary>
	/// Calculate squared length of this vector.
	/// </summary>
	/// <returns>The length of this vector.</returns>
	FORCEINLINE float lengthSqr() const
	{
		return x*x + y*y + z*z;
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

	/// <summary>
	/// Returns normalized vector
	/// </summary>
	FORCEINLINE static Vector3 normalize(Vector3 vector)
	{
		vector.normalize();
		return vector;
	}

	/// <summary>
	/// Returns normalized vector
	/// </summary>
	FORCEINLINE static void normalize(Vector3 vector, Vector3* result)
	{
		*result = normalize(vector);
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static float length(Vector3 vector)
	{
		return vector.length();
	}

	/// <summary>
	/// Calculate length of vector
	/// </summary>
	FORCEINLINE static void length(Vector3 vector, float* result)
	{
		*result = length(vector);
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static float dot(Vector3& a, Vector3& b)
	{
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/// <summary>
	/// Dot product of two vectors
	/// </summary>
	FORCEINLINE static void dot(Vector3& a, Vector3& b, float* result)
	{
		*result = dot(a, b);
	}

	/// <summary>
	/// Cross product of two vectors
	/// </summary>
	FORCEINLINE static Vector3 cross(Vector3& a, Vector3& b)
	{
		return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
	}

	/// <summary>
	/// Cross product of two vectors
	/// </summary>
	FORCEINLINE static void cross(Vector3& a, Vector3& b, Vector3* result)
	{
		*result = cross(a, b);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static Vector3 lerp(Vector3& a, Vector3& b, float t) 
	{
		return Vector3(a.x * (1 - t) + t * b.x, a.y * (1 - t) + t * b.y, a.z * (1 - t) + t * b.z);
	}

	/// <summary>
	/// Linerally interpolates between two vectors
	/// </summary>
	FORCEINLINE static void lerp(Vector3& a, Vector3& b, float t, Vector3* result)
	{
		*result = lerp(a, b, t);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static Vector3 negate(Vector3& a)
	{
		return Vector3(a.x * -1.0f, a.y * -1.0f, a.z * -1.0f);
	}

	/// <summary>
	/// Negation of the vector
	/// </summary>
	FORCEINLINE static void negate(Vector3& a, Vector3* result)
	{
		*result = negate(a);
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static Vector3 abs(Vector3& a)
	{
		return Vector3(fabs(a.x), fabs(a.y), fabs(a.z));
	}

	/// <summary>
	/// Absolute value of the vector
	/// </summary>
	FORCEINLINE static void abs(Vector3& a, Vector3* result)
	{
		*result = abs(a);
	}

	/// <summary>
	/// Distance between two vectors
	/// </summary>
	FORCEINLINE static float distance(Vector3& a, Vector3& b)
	{
		return sqrtf((b.x - a.x)*(b.x - a.x) + (b.y - a.y)*(b.y - a.y) + (b.z - a.z)*(b.z - a.z));
	}

	/// <summary>
	/// Distance between two vectors
	/// </summary>
	FORCEINLINE static void distance(Vector3& a, Vector3& b, float* result)
	{
		*result = distance(a, b);
	}

public:
	void Vector3::operator -= (Vector3 r){
		x -= r.x;
		y -= r.y;
		z -= r.z;
	}

	void operator += (Vector3 r) {
		x += r.x;
		y += r.y;
		z += r.z;
	}

	void Vector3::operator *= (float value) {
		x *= value;
		y *= value;
		z *= value;
	}

	/// <summary>
	/// Result = Vector - Vector[r] operator
	/// </summary>
	Vector3 Vector3::operator - (Vector3 r) const
	{
		return Vector3(x - r.x, y - r.y, z - r.z);
	}

	/// <summary>
	/// Result = Vector + Vector[r] operator
	/// </summary>
	Vector3 Vector3::operator + (Vector3 r) const
	{
		return Vector3(x + r.x, y + r.y, z + r.z);
	}

	/// <summary>
	/// Result = Vector * Vector[r] operator
	/// </summary>
	Vector3 Vector3::operator * (Vector3 r) const
	{
		return Vector3(x * r.x, y * r.y, z * r.z);
	}

	/// <summary>
	/// Result = Vector * float operator
	/// </summary>
	Vector3 Vector3::operator * (float r) const
	{
		return Vector3(x * r, y * r, z * r);
	}

public:
	/// <summary>
	/// X compoennt
	/// </summary>
	float x;

	/// <summary>
	/// Y component
	/// </summary>
	float y;

	/// <summary>
	/// Z component
	/// </summary>
	float z;
};

#endif // Vector3_H