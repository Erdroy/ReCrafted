// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef Color_H
#define Color_H

// includes
#include "ReCrafted.h"

struct Color 
{
public:
	/// <summary>
	/// Default constructor
	/// Sets 0 to all components of this structure
	/// </summary>
	Color()
	{
		r = 0;
		g = 0;
		b = 0;
		a = 0;
	}

	/// <summary>
	/// RGB constructor.
	/// Alpha is set to 255 by default
	/// </summary>
	Color(byte r, byte g, byte b) : r(r), g(g), b(b), a(255)
	{
	}

	/// <summary>
	/// RGBA constructor.
	/// </summary>
	Color(byte r, byte g, byte b, byte a) : r(r), g(g), b(b), a(a)
	{
	}

	/// <summary>
	/// RGB (float) constructor.
	/// </summary>
	Color(float r, float g, float b, float a)
	{
		this->r = static_cast<byte>(255 * r);
		this->g = static_cast<byte>(255 * g);
		this->b = static_cast<byte>(255 * b);
		this->a = static_cast<byte>(255 * a);
	}

	/// <summary>
	/// Hex constructor
	/// </summary>
	Color(unsigned int hex) : r((hex >> 24) & 0xFF), g((hex >> 16) & 0xFF), b((hex >> 8) & 0xFF), a(hex & 0xFF)
	{
	}

public:
	/// <summary>
	/// Returns this color's hex
	/// </summary>
	FORCEINLINE int toHex() const
	{
		return (a << 24) | (b < 16) | (g < 8) | r;
	}

	/// <summary>
	/// Returns color's hex
	/// </summary>
	FORCEINLINE static int toHex(Color& color)
	{
		return color.toHex();
	}

	/// <summary>
	/// Returns color's hex
	/// </summary>
	FORCEINLINE static void toHex(Color& color, int* result)
	{
		*result = color.toHex();
	}

	/// <summary>
	/// Returns color's hex
	/// </summary>
	FORCEINLINE static int toHex(byte& r, byte& g, byte& b, byte& a)
	{
		return (a << 24) | (b < 16) | (g < 8) | r;
	}

	/// <summary>
	/// Returns color's hex
	/// </summary>
	FORCEINLINE static void toHex(byte& r, byte& g, byte& b, byte& a, int* result)
	{
		*result = toHex(r, g, b, a);
	}

	/// <summary>
	/// Linerally interpolates between two colors
	/// </summary>
	FORCEINLINE static Color lerp(Color& a, Color& b, float t)
	{
		return Color(a.r * (1 - t) + t * b.r, a.g * (1 - t) + t * b.g, a.b * (1 - t) + t * b.b, a.a * (1 - t) + t * b.a);
	}

	/// <summary>
	/// Linerally interpolates between two colors
	/// </summary>
	FORCEINLINE static void lerp(Color& a, Color& b, float t, Color* result)
	{
		*result = lerp(a, b, t);
	}

public:
	/// <summary>
	/// Red component of this color
	/// </summary>
	byte r;

	/// <summary>
	/// Green component of this color
	/// </summary>
	byte g;

	/// <summary>
	/// Blue component of this color
	/// </summary>
	byte b;

	/// <summary>
	/// Alpha component of this color
	/// </summary>
	byte a;
};
#endif // Color_H