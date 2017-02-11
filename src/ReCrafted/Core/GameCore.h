// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef GAMECORE_H
#define GAMECORE_H

// includes
#include "GameBase.h"
#include "../Graphics/Rendering.h"
#include "../Graphics/Camera.h"

/// <summary>
/// GameCoreClass
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

	Ptr<Camera> m_camera = nullptr;

public:
	GameCore() { m_instance = this; }

	void onLoad() override;

	void onUnload() override;

	void onResize(uint width, uint height) override;

	void onUpdate() override;

	void onSimulate() override;

	void onDraw() override;
	
public:
	FORCEINLINE static uint getWindowWidth()
	{
		return m_instance->m_width;
	}

	FORCEINLINE static uint getWindowHeight()
	{
		return m_instance->m_height;
	}

	FORCEINLINE static float getAspectRatio()
	{
		return float(m_instance->m_width) / float(m_instance->m_height);
	}
};

#endif // GAMECORE_H