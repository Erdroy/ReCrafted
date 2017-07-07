// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TIME_H
#define TIME_H

// includes
#include "Utils/Defines.h"
#include "Common/ReCraftedAPI.h"

/// <summary>
/// Time class.
/// </summary>
class Time
{
	API_DEF

	friend class GameMain;

private:
	static Time* m_instance;

private:
	double m_deltaTime = 0.0;
	float m_time = 0.0;
	int m_frames = 0;

public:
	/// <summary>
	/// Default constuctor for Time class.
	/// </summary>
	Time() { m_instance = this; }

public:
	/// <summary>
	/// Dispose the time.
	/// </summary>
	FORCEINLINE void dispose()
	{
		m_deltaTime = 0.0f;

		// suicide
		delete this;
	}

public:
	/// <summary>
	/// The current frame delta time.
	/// Use for 'Update' calls.
	/// </summary>
	/// <returns>The frame delta time.</returns>
	FORCEINLINE static double deltaTime()
	{
		return m_instance->m_deltaTime;
	}

	/// <summary>
	/// The current game time.
	/// </summary>
	/// <returns>The time.</returns>
	FORCEINLINE static float time()
	{
		return m_instance->m_time;
	}

	/// <summary>
	/// The current game frame count.
	/// </summary>
	/// <returns>The frame count.</returns>
	FORCEINLINE static int frames()
	{
		return m_instance->m_frames;
	}
};

#endif // TIME_H