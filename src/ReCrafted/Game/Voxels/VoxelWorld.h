// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELWORLD_H
#define VOXELWORLD_H

// includes
#include "../../recraftedPrerequisites.h"
#include "VoxelChunk.h"
#include "../../Core/Logger.h"
#include "VoxelChunkTree.h"

/*
WORLD SPACE:
	NW	N  NE
	 W  ^  E
	SW	S  SE
*/

class VoxelWorld
{
private:
	std::vector<VoxelChunk*> m_chunks = {};

	VoxelChunkTree* m_chunkTree = nullptr;
	std::vector<VoxelChunk*> m_visibleChunks = {};

private:

public:
	void init(bool generateworld);

	void update();

	void simulate();
	
	void draw();

	void findNeighs(VoxelChunk* chunk);

	void initializeNeighs(VoxelChunk* chunk);

	VoxelChunk* getVoxelChunk(Vector3 containedPoint);

	FORCEINLINE VoxelChunk* getVoxelChunk(Vector3i position)
	{
		
	}

	FORCEINLINE bool raycast(Vector3 origin, Vector3 direction, voxelid* voxelid, Vector3* point, Vector3* normal);

	FORCEINLINE void dispose()
	{
		for(auto && chunk : m_chunks)
		{
			if (chunk == nullptr)
				continue;

			chunk->dispose();
			delete chunk;
		}
		m_chunks.clear();
		Logger::write("World unloaded", LogLevel::Info);
	}
};

#endif // VOXELWORLD_H