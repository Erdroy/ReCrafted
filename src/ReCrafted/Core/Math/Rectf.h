// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef Rectf_H
#define Rectf_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Core/Math/Vector2.h"

struct Rectf
{
public:
	/// <summary>
	/// Default constructor
	/// Sets 0 to all components of this structure
	/// </summary>
	Rectf()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	Rectf(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{

	}

public:
	/// <summary>
	/// Size of this Rectangle
	/// </summary>
	FORCEINLINE Vector2 size() const
	{
		return Vector2(width, height);
	}

	/// <summary>
	/// Size of this Rectangle
	/// </summary>
	FORCEINLINE void size(const Vector2& size)
	{
		width = size.x;
		height = size.y;
	}

	FORCEINLINE Vector2 center() const
	{
		return Vector2(x, y);
	}

	FORCEINLINE Vector2 center(const Vector2& center)
	{
		x = center.x;
		y = center.y;
	}

	/// <summary>
	/// Minimum X coordinate of this Rectangle
	/// </summary>
	FORCEINLINE float left() const
	{
		return x - (width / 2);
	}

	/// <summary>
	/// Maximum X coordinate of this Rectangle
	/// </summary>
	FORCEINLINE float right() const
	{
		return x + (width / 2);
	}

	/// <summary>
	/// Minimum Y coordinate of this Rectangle
	/// </summary>
	FORCEINLINE float bottom() const
	{
		return y - (height / 2);
	}

	/// <summary>
	/// Maximum Y coordinate of this Rectangle
	/// </summary>
	FORCEINLINE float top() const
	{
		return y + (height / 2);
	}

	/// <summary>
	/// Check if two Rectangles overlap each other
	/// </summary>
	FORCEINLINE bool overlaps(Rectf& a, Rectf& b)
	{
		return (a.left < b.right && a.right > b.left && a.top < b.bottom && a.bottom > b.top);
	}

	/// <summary>
	/// Check if two Rectangles overlap each other
	/// </summary>
	FORCEINLINE void overlaps(Rectf& a, Rectf& b, bool* result)
	{
		*result = overlaps(a, b);
	}

public:
	/// <summary>
	/// X coordinate of this Rectangle
	/// </summary>
	float x;

	/// <summary>
	/// Y coordinate of this Rectangle
	/// </summary>
	float y;

	/// <summary>
	/// Width of this Rectangle
	/// </summary>
	float width;

	/// <summary>
	/// Height of this Rectangle
	/// </summary>
	float height;
};

#endif // Rectf_H