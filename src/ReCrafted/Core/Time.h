// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef TIME_H
#define TIME_H

// includes
#include "../Utils/Defines.h"

class Time
{
private:
	static Time* m_instance;

private:
	float m_deltaTime = 0.0f;

public:
	Time() { m_instance = this; }

	FORCEINLINE void setDeltaTime(float deltaTime)
	{
		m_deltaTime = deltaTime;
	}

	FORCEINLINE void dispose()
	{
		m_deltaTime = 0.0f;

		// suicide
		delete this;
	}

public:
	FORCEINLINE static float deltaTime()
	{
		return m_instance->m_deltaTime;
	}
};

#endif // TIME_H