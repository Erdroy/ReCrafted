// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef GAMEMAIN_H
#define GAMEMAIN_H

// includes
#include "../Graphics/Rendering.h"
#include "../Graphics/Camera.h"
#include "../Game/Universe.h"
#include "../Common/Input.h"
#include "../Common/Time.h"
#include "../Graphics/UI/UI.h"

class GameMain
{
	API_DEF

private:
	static GameMain* m_instance;

private:
	volatile bool m_running = false;
	bool m_initialized = false;

	Rendering* m_rendering = nullptr;
	UI* m_ui = nullptr;

	Universe* m_universe = nullptr;
	Logger* m_logger = nullptr;
	Input* m_input = nullptr;
	Time* m_time = nullptr;

	Ptr<Camera> m_camera = nullptr;

	uint m_tickrate = 60u;

public:
	/// <summary>
	/// Run game
	/// </summary>
	void run();

	/// <summary>
	/// Shutdown the game
	/// </summary>
	void shutdown();

public:
	void onLoad();
	void onUnload();
	void onResize(uint width, uint height);
	void onUpdate();
	void onSimulate();
	void onDraw();

public:
	/// <summary>
	/// Shutdowns the game.
	/// </summary>
	FORCEINLINE static void quit()
	{
		m_instance->m_running = false;
	}

	/// <summary>
	/// Set the tick rate of simulation.
	/// </summary>
	/// <param name="ticksPerSecond">The amount of ticks per second. Default: 60</param>
	FORCEINLINE static void setSimulationTickrate(uint ticksPerSecond)
	{
		m_instance->m_tickrate = ticksPerSecond;
	}

	/// <summary>
	/// Gets the tick rate of simulation.
	/// </summary>
	FORCEINLINE static uint getSimulationTickrate()
	{
		return m_instance->m_tickrate;
	}

	/// <summary>
	/// Check if the engine is running.
	/// </summary>
	/// <returns>Returns true when is running.</returns>
	FORCEINLINE static bool isRunning()
	{
		return m_instance->m_running;
	}
};

#endif // GAMEMAIN_H
