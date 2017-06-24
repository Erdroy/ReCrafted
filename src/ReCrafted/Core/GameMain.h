// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef GAMEMAIN_H
#define GAMEMAIN_H

// includes
#include "Input.h"
#include "Time.h"
#include "../Graphics/Rendering.h"
#include "../Graphics/Camera.h"
#include "../Game/Universe.h"

class GameMain
{
private:
	static GameMain* m_instance;

private:
	volatile bool m_running = false;
	bool m_initialized = false;

	uint m_width = 1280;
	uint m_height = 720;

	Rendering* m_rendering = nullptr;
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
	/// Set the tick rate of simulation.
	/// </summary>
	/// <param name="ticksPerSecond">The amount of ticks per second. Default: 60</param>
	FORCEINLINE static void setSimulationTickrate(uint ticksPerSecond)
	{
		m_instance->m_tickrate = ticksPerSecond;
	}

	/// <summary>
	/// Check if the engine is running.
	/// </summary>
	/// <returns>Returns true when is running.</returns>
	FORCEINLINE static bool isRunning()
	{
		return m_instance->m_running;
	}

	/// <summary>
	/// The game window width.
	/// </summary>
	/// <returns>The width.</returns>
	FORCEINLINE static uint getWindowWidth()
	{
		return m_instance->m_width;
	}

	/// <summary>
	/// The game window height.
	/// </summary>
	/// <returns>The height.</returns>
	FORCEINLINE static uint getWindowHeight()
	{
		return m_instance->m_height;
	}

	/// <summary>
	/// The game window aspect ratio.
	/// </summary>
	/// <returns>The aspect ratio.</returns>
	FORCEINLINE static float getAspectRatio()
	{
		return float(m_instance->m_width) / float(m_instance->m_height);
	}
};

#endif // GAMEMAIN_H
