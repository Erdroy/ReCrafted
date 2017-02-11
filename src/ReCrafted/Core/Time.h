// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TIME_H
#define TIME_H

// includes
#include "../Utils/Defines.h"

class Time
{
	friend class GameCore;

private:
	static Time* m_instance;

private:
	double m_deltaTime = 0.0;
	float m_time = 0.0;

public:
	Time() { m_instance = this; }

	FORCEINLINE void dispose()
	{
		m_deltaTime = 0.0f;

		// suicide
		delete this;
	}

public:
	FORCEINLINE static double deltaTime()
	{
		return 1.0 / m_instance->m_deltaTime;
	}

	FORCEINLINE static float time()
	{
		return m_instance->m_time;
	}
};

#endif // TIME_H