// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "../recraftedPrerequisites.h"
#include "../Graphics/Mesh.h"
#include "Voxels/VoxelWorld.h"

/// <summary>
/// Universe class - main class for whole game world.
/// Manages voxel engine, AI, players, handles multiplayer etc.
/// </summary>
class Universe
{
	friend class GameCore;

private:
	static Universe* m_instance;

private:
	Ptr<VoxelWorld> m_currentWorld = nullptr;

private:
	void init();

private:
	Universe() { m_instance = this; }

public:
	void update();

	void simulate();

	void drawShadowCasters();

	void draw();

	FORCEINLINE void dispose()
	{
		m_currentWorld->dispose();
	}
};

#endif // UNIVERSE_H