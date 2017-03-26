// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef GAMECORE_H
#define GAMECORE_H

// includes
#include "../recraftedPrerequisites.h"
#include "GameBase.h"
#include "../Graphics/Rendering.h"
#include "../Graphics/Camera.h"
#include "../Game/Universe.h"
#include "Logger.h"

/// <summary>
/// GameCore class
/// </summary>
class GameCore : public GameBase
{
private:
	static GameCore* m_instance;

private:
	uint m_width = 1280;
	uint m_height = 720;

	bool m_initialized = false;
	Rendering* m_rendering = nullptr;
	Universe* m_universe = nullptr;
	Logger* m_logger = nullptr;
	Ptr<Camera> m_camera = nullptr;

public:
	/// <summary>
	/// Default constructor of GameCore class.
	/// </summary>
	GameCore() { m_instance = this; }

public:
	void onLoad() override;

	void onUnload() override;

	void onResize(uint width, uint height) override;

	void onUpdate() override;

	void onSimulate() override;

	void onDraw() override;
	
public:
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

#endif // GAMECORE_H