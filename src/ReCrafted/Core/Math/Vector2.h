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