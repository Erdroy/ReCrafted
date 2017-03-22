// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef Rect_H
#define Rect_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Core/Math/Vector2.h"

struct Rect 
{
public:
	/// <summary>
	/// Default constructor
	/// Sets 0 to all components of this structure
	/// </summary>
	Rect() 
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	Rect(int x, int y, int width, int height)  : x(x), y(y), width(width), height(height)
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
		width = static_cast<int>(size.x);
		height = static_cast<int>(size.y);
	}

	/// <summary>
	/// Center of this Rectangle
	/// </summary>
	FORCEINLINE Vector2 center() const 
	{
		return Vector2(x, y);
	}

	/// <summary>
	/// Center of this Rectangle
	/// </summary>
	FORCEINLINE Vector2 center(const Vector2& center)
	{
		x = static_cast<int>(center.x);
		y = static_cast<int>(center.y);
	}

	/// <summary>
	/// Minimum X coordinate of this Rectangle
	/// </summary>
	FORCEINLINE int left() const 
	{
		return x - (width / 2);
	}

	/// <summary>
	/// Maximum X coordinate of this Rectangle
	/// </summary>
	FORCEINLINE int right() const
	{
		return x + (width / 2);
	}

	/// <summary>
	/// Minimum Y coordinate of this Rectangle
	/// </summary>
	FORCEINLINE int bottom() const
	{
		return y - (height / 2);
	}

	/// <summary>
	/// Maximum Y coordinate of this Rectangle
	/// </summary>
	FORCEINLINE int top() const
	{
		return y + (height / 2);
	}

	/// <summary>
	/// Check if two Rectangles overlap each other
	/// </summary>
	FORCEINLINE bool overlaps(Rect& a, Rect& b) 
	{
		return (a.left < b.right && a.right > b.left && a.top < b.bottom && a.bottom > b.top);
	}

	/// <summary>
	/// Check if two Rectangles overlap each other
	/// </summary>
	FORCEINLINE void overlaps(Rect& a, Rect& b, bool* result) 
	{
		*result = overlaps(a, b);
	}

public:
	/// <summary>
	/// X coordinate of this Rectangle
	/// </summary>
	int x;

	/// <summary>
	/// Y coordinate of this Rectangle
	/// </summary>
	int y;

	/// <summary>
	/// Width of this Rectangle
	/// </summary>
	int width;

	/// <summary>
	/// Height of this Rectangle
	/// </summary>
	int height;
};

#endif // Rect_H