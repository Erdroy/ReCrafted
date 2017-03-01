// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
#pragma once

#ifndef MATH_H
#define MATH_H

// includes
#include <cmath>

#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"

#define PI 3.14159265358979323846

typedef Vector2 vec2;
typedef Vector3 vec3;
//typedef Vector4 vec4;
typedef Matrix mat4;

typedef Vector2 vector2;
typedef Vector3 vector3;
//typedef Vector4 vector4;
typedef Matrix matrix4;

typedef Vector2 float2;
typedef Vector3 float3;
//typedef Vector4 float4;
typedef Matrix float4x4;

namespace internal{
	const float degtorad = float(PI / 180.0f);
	const float radtodeg = float(180.0f / PI);
}

class Math
{

public:
	template<class T>
	FORCEINLINE static T clmap(T v, T min, T max)
	{
		if (v < min)
			return min;

		if (v > max)
			return max;

		return v;
	}

	FORCEINLINE static int sign(int x)
	{
		return x < 0 ? -1 : 1;
	}

	FORCEINLINE static float signf(float x)
	{
		return x < 0.0f ? -1.0f : 1.0f;
	}

	FORCEINLINE static float minf(float a, float b)
	{
		return a < b ? a : b;
	}

	FORCEINLINE static float maxf(float a, float b)
	{
		return a > b ? a : b;
	}

	FORCEINLINE static float cosx(float x)
	{
		return cos(x); // TODO: optimize cos
	}

	FORCEINLINE static float sinx(float x)
	{
		return sin(x); // TODO: optimize sin
	}

	FORCEINLINE static float degreeToRadian(float degree)
	{
		return degree * internal::degtorad;
	}

	FORCEINLINE static float radianToDegree(float radiand)
	{
		return radiand * internal::radtodeg;
	}
};

#endif // MATH_H