// ReCrafted © 2016-2017 Always Too Late
#pragma once

#ifndef MATH_H
#define MATH_H

// includes
#include <cmath>

#include "Matrix.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#define PI 3.14159265358979323846

namespace internal{
	const float degtorad = float(PI / 180.0f);
	const float radtodeg = float(180.0f / PI);
}

class Math
{

public:
	template<class T>
	FORCEINLINE static T clamp(T v, T min, T max)
	{
		if (v < min)
			return min;

		if (v > max)
			return max;

		return v;
	}

    FORCEINLINE static int pow(int x, int n)
    {
        return static_cast<int>(::pow(static_cast<float>(x), n));
    }

    FORCEINLINE static float powf(float x, int n)
    {
        return ::pow(x, n);
    }

	FORCEINLINE static int sign(int x)
	{
		return x < 0 ? -1 : 1;
	}

	FORCEINLINE static float signf(float x)
	{
		return x < 0.0f ? -1.0f : 1.0f;
	}

	FORCEINLINE static int signfz(float x)
	{
		return x > 0 ? 1 : x < 0 ? -1 : 0;
	}

	FORCEINLINE static float minf(float a, float b)
	{
		return a < b ? a : b;
	}

	FORCEINLINE static float maxf(float a, float b)
	{
		return a > b ? a : b;
	}

	FORCEINLINE static float sqrtf(float x)
	{
		return sqrt(x);
	}

	FORCEINLINE static float tanf(float x)
	{
		return tan(x);
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

	FORCEINLINE static unsigned long roundUpToPow2(unsigned long v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;

		return v;
	}

	FORCEINLINE static int roundUp(int numToRound, int multiple)
	{
		// source: http://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number

		// allow round up to 0
		if (numToRound < 0 && -numToRound < multiple)
			return 0;

		auto remainder = numToRound % multiple;

		if (remainder == 0)
			return numToRound;

		return numToRound + multiple - remainder;
	}
};

#endif // MATH_H