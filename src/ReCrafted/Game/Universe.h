// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef UNIVERSE_H
#define UNIVERSE_H

// includes
#include "../recraftedPrerequisites.h"
#include "../Graphics/Mesh.h"
#include "../Core/Logger.h"
#include "Voxels/VoxelWorld.h"
#include "Voxels/VoxelChunkProcessor.h"

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
	Ptr<VoxelWorld> m_currentWorld = nullptr;
	Ptr<VoxelChunkProcessor> m_chunkProcessor = nullptr;

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

public:
	/// <summary>
	/// Gets the current voxel world.
	/// </summary>
	/// <returns>The current world pointer.</returns>
	FORCEINLINE static VoxelWorld* getCurrentWorld()
	{
		return m_instance->m_currentWorld.get();
	}
};

#endif // UNIVERSE_H