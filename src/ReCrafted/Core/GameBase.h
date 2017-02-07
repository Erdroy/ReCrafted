// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef GAMEBASE_H
#define GAMEBASE_H

// includes

#include "../Utils/Types.h"
#include "../bgfxPrerequisites.h"
#include "Input.h"
#include "Time.h"

/// <summary>
/// GameBase base class for game 'main-class'.
/// </summary>
class GameBase
{
private:
	static GameBase* m_instance;

private:
	bool m_running = false;
	Input* m_input = nullptr;
	Time* m_time = nullptr;

	uint m_tickrate = 60u;

public:
	GameBase() {  }
	virtual ~GameBase() { }

	/// <summary>
	/// Run game
	/// </summary>
	void run();

	/// <summary>
	/// Shutdown the game
	/// </summary>
	void shutdown();

#pragma region virtual methods
	virtual void onLoad() = 0;

	virtual void onUnload() = 0;

	virtual void onResize(uint width, uint height) = 0;

	virtual void onUpdate() = 0;

	virtual void onSimulate() = 0;

	virtual void onDraw() = 0;
#pragma endregion

public:
	/// <summary>
	/// Set the tick rate of simulation.
	/// </summary>
	/// <param name="ticksPerSecond">The amount of ticks per second. Default: 60</param>
	FORCEINLINE static void setSimulationTickrate(uint ticksPerSecond)
	{
		m_instance->m_tickrate = ticksPerSecond;
	}
};

#endif // GAMEBASE_H