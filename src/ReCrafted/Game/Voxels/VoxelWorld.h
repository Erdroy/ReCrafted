// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELWORLD_H
#define VOXELWORLD_H

// includes
#include "../../recraftedPrerequisites.h"
#include "VoxelChunk.h"

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

private:
public:
	void init(bool generateworld);

	void update();

	void simulate();
	
	void draw();

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
	}
};

#endif // VOXELWORLD_H