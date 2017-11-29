// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef GAMEMAIN_H
#define GAMEMAIN_H

// includes
#include "recraftedConfig.h"
#include "Common/Input.h"
#include "Common/Time.h"
#include "Game/Universe.h"
#include "Graphics/Rendering.h"
#include "Graphics/Camera.h"
#include "Graphics/UI/UI.h"
#include "Scripting/Domain.h"
#include "Scripting/Method.h"
#include "Common/EntityPool.h"
#include "Logger.h"

class GameMain
{
	API_DEF

	friend class Rendering;

private:
	static GameMain* m_instance;

private:
	volatile bool m_running = false;
	bool m_initialized = false;
	bool m_wireframe = false;

	EntityPool* m_entityPool = nullptr;
	Rendering* m_rendering = nullptr;
	Universe* m_universe = nullptr;
	Logger* m_logger = nullptr;
	Input* m_input = nullptr;
	Time* m_time = nullptr;
	UI* m_ui = nullptr;
	uint m_tickrate = 60u;

	bool m_showCursor = true;
	bool m_lockCursor = true;

	int m_targetFps = 60;
	int m_simulationFps = 66;

	// scripting

	Ptr<Domain> m_domain = nullptr;
	Ptr<Object> m_gamemain = nullptr;

	// methods

	Ptr<Method> m_init_method = nullptr;
	Ptr<Method> m_update_method = nullptr;
	Ptr<Method> m_simulate_method = nullptr;
	Ptr<Method> m_drawui_method = nullptr;
	Ptr<Method> m_shutdown_method = nullptr;

private:
	void initScripting();
	void waitForTargetFps(double last);

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
	void onCursorRequest();

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
	/// Sets the cursor state.
	/// </summary>
	FORCEINLINE static void setShowCursor(bool state)
	{
		m_instance->m_showCursor = state;
	}

	/// <summary>
	/// Gets the cursor state.
	/// </summary>
	FORCEINLINE static bool getShowCursor()
	{
		return m_instance->m_showCursor;
	}

	/// <summary>
	/// Sets the cursor lock mode.
	/// </summary>
	FORCEINLINE static void setLockCursor(bool state)
	{
		m_instance->m_lockCursor = state;
	}

	/// <summary>
	/// Gets the cursor lock mode.
	/// </summary>
	FORCEINLINE static bool getLockCursor()
	{
		return m_instance->m_lockCursor;
	}

	FORCEINLINE static void setTargetFps(int targetFps)
	{
		m_instance->m_targetFps = targetFps;
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
