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
	VoxelChunkTree* m_chunkTree = nullptr;
	std::vector<VoxelChunk*> m_visibleChunks = {};

private:

public:
	void init(bool generateworld);

	void update();

	void simulate();
	
	void draw();

	void findNeighs(VoxelChunk* chunk);

	void generateNeigs(VoxelChunk* chunk);

	FORCEINLINE VoxelChunk* generateChunk(int x, int z)
	{
		auto chunk = new VoxelChunk;
		chunk->world = this;
		chunk->m_x = x;
		chunk->m_z = z;

		// generate data
		chunk->dataGenerate();

		// add to chunk tree
		m_chunkTree->add(chunk);

		return chunk;
	}

	VoxelChunk* getVoxelChunk(Vector3 containedPoint);

	VoxelChunk* getVoxelChunk(Vector3i position);

	bool raycast(Vector3 origin, Vector3 direction, voxelid* voxelid, Vector3* point, Vector3* normal);

	FORCEINLINE void dispose()
	{
		// dispose chunk tree
		m_chunkTree->dispose();
		Logger::write("World unloaded", LogLevel::Info);
	}
};

#endif // VOXELWORLD_H