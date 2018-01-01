// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "World/SpaceObject/SpaceObject.h"

/// <summary>
/// Universe class - main class for whole game world.
/// Manages voxel engine, AI, players, handles multiplayer etc.
/// </summary>
class Universe
{
	friend class GameMain;

private:
	static Universe* m_instance;

private:
	Ptr<SpaceObject> m_testObject1 = nullptr;

private:
	void init();

private:
	Universe() { m_instance = this; }

public:
	/// <summary>
	/// Updates the whole universe.
	/// </summary>
	void update();

	/// <summary>
	/// Simulates the whole universe.
	/// </summary>
	void simulate();

	/// <summary>
	/// Draws all shadow casters.
	/// </summary>
	void drawShadowCasters();

	/// <summary>
	/// Draws the whole universe.
	/// </summary>
	void draw();

	/// <summary>
	/// Disposes the universe.
	/// </summary>
	void dispose();
};

#endif // UNIVERSE_H