// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef VOXELGENERATOR_H
#define VOXELGENERATOR_H

// includes
#include "ReCrafted.h"
#include "Voxels/VoxelChunk.h"

class VoxelGenerator
{
public:
	static void initialize();

	static void beginChunk(int x, int z);

	static void endChunk();
	
	static voxelid generate(int index, int y);

	FORCEINLINE static voxelid generate(int x, int y, int z)
	{
		return generate(x * ChunkWidth + z, y);
	}
};

#endif // VOXELGENERATOR_H
