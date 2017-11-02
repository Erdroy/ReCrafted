// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef VOXELWORLD_H
#define VOXELWORLD_H

// includes
#include "ReCrafted.h"
#include "Core/Logger.h"
#include "VoxelChunk.h"
#include "VoxelMap.h"

/*
WORLD SPACE:
	NW	N  NE
	 W  ^  E
	SW	S  SE
*/

/// <summary>
/// Raycast hit result data structure.
/// </summary>
struct RaycastHit
{
public:
	voxelid voxel = voxel_air;
	Vector3 position = {};
	Vector3 normal = {};
};

/// <summary>
/// VoxelWorld class - handles whole current world, voxel chunks etc.
/// </summary>
class VoxelWorld
{
	friend class Universe;
	friend class VoxelChunk;

private:
	static int m_availableUploads;
	static VoxelWorld* m_instance;

private:
	VoxelMap* m_chunkMap = nullptr;

private:
	void update_digplace();

public:
	VoxelWorld()
	{
		m_instance = this;
	}

public:
	/// <summary>
	/// Initializes voxel world
	/// </summary>
	void init();

	/// <summary>
	/// Updates th whole world
	/// </summary>
	void update();

	/// <summary>
	/// Simulates the whole world
	/// </summary>
	void simulate();
	
	/// <summary>
	/// Draws visible objects in the whole world
	/// </summary>
	void draw();

	/// <summary>
	/// Generates chunk
	/// </summary>
	/// <param name="x">The chunk x coord in chunk-space.</param>
	/// <param name="z">The chunk z coord in chunk-space.</param>
	/// <returns>The created and queued for generation chunk.</returns>
	VoxelChunk* generateChunk(int x, int z);

	/// <summary>
	/// Finds chunk which contains the point.
	/// </summary>
	/// <param name="containedPoint">The point.</param>
	/// <returns>Found chunk or null when chunk is does not exist.</returns>
	VoxelChunk* getVoxelChunk(Vector3 containedPoint) const;

	/// <summary>
	/// Raycasts trough voxels.
	/// </summary>
	/// <param name="origin">The ray origin.</param>
	/// <param name="direction">The ray direction.</param>
	/// <param name="length">The ray length.</param>
	/// <param name="hit">The ray result data.</param>
	/// <returns>True when ray hits something.</returns>
	bool raycast(Vector3 origin, Vector3 direction, float length, RaycastHit* hit);

	/// <summary>
	/// Disposes the world
	/// </summary>
	FORCEINLINE void dispose() const
	{
		// dispose chunk map
		m_chunkMap->dispose();
		Logger::write("World unloaded", LogLevel::Info);
	}

public:
	static VoxelWorld* instance()
	{	
		return m_instance;
	}
};

#endif // VOXELWORLD_H