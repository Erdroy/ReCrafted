// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunk.h"
#include "../../Core/Profiler.h"

void VoxelChunk::worker_dataGenerate()
{
	Profiler::beginProfile();
	for(auto y = 0; y < ChunkHeight; y ++)
	{
		for(auto x = 0; x < ChunkWidth; x ++)
		{
			for (auto z = 0; z < ChunkWidth; z++)
			{
				if(y == 0)
					m_voxels[x][y][z] = 1u;
				else
					m_voxels[x][y][z] = 0u;
			}
		}
	}
	Profiler::endProfile("Chunk voxel data generated in %0.7f ms.");
}

void VoxelChunk::worker_meshGenerate()
{
}

void VoxelChunk::update()
{
}

void VoxelChunk::simulate()
{
}
