// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELWORLD_H
#define VOXELWORLD_H

// includes
#include "../../recraftedPrerequisites.h"
#include "../../Core/Logger.h"
#include "VoxelChunk.h"
#include "VoxelChunkMap.h"

/*
WORLD SPACE:
	NW	N  NE
	 W  ^  E
	SW	S  SE
*/

class VoxelWorld
{
	friend class Universe;

private:

	VoxelChunkMap* m_chunkMap = nullptr;

private:

public:
	void init(bool generateworld);

	void update();

	void simulate();
	
	void draw();

	VoxelChunk* generateChunk(int x, int z);

	VoxelChunk* getVoxelChunk(Vector3 containedPoint);

	VoxelChunk* getVoxelChunk(Vector3i position);

	void findNeighs(VoxelChunk* chunk);

	bool raycast(Vector3 origin, Vector3 direction, voxelid* voxelid, Vector3* point, Vector3* normal);

	FORCEINLINE void dispose() const
	{
		// dispose chunk map
		m_chunkMap->dispose();
		Logger::write("World unloaded", LogLevel::Info);
	}
};

#endif // VOXELWORLD_H