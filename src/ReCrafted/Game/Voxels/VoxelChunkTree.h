// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNKTREE_H
#define VOXELCHUNKTREE_H

// includes
#include "../../Graphics/Camera.h"
#include "VoxelChunk.h"
#include <vector>

class VoxelChunkTree
{
private:
	static const int tableWidth = 64;
	static const int tableWidthHalf = 32;

private:
	struct vctEntry
	{
	public:
		// root/vct space
		int x = 0;
		int z = 0;
		
		VoxelChunk* chunkTable[tableWidth * tableWidth] = {};

		int getChunkIndex(int x, int z) const
		{
		}

		VoxelChunk* getChunk(int x, int z)
		{
		}

		bool contains(VoxelChunk* chunk) const
		{
		}

		void dispose() const
		{
			// dispose all chunks
			for (auto i = 0; i < tableWidth * tableWidth; i++)
			{
				auto chunk = chunkTable[i];

				if (chunk)
				{
					chunk->dispose();
				}
			}

			// suicide
			delete this;
		}
	};
private:
	std::vector<vctEntry*> m_roots = {};
	std::vector<VoxelChunk*> m_chunks = {};


public:
	void add(VoxelChunk* chunk)
	{
		m_chunks.push_back(chunk);

		auto rootX = (chunk->m_x - tableWidth) / tableWidth;
		auto rootZ = (chunk->m_z - tableWidth) / tableWidth;

		vctEntry* root = nullptr;
		for(auto current : m_roots)
		{
			
		}
	}

	void getNearChunks(Vector2 point, float distance, std::vector<VoxelChunk*>* chunks)
	{
	}

	void getNearChunksCulled(Vector3 point, float distance, Camera* camera)
	{

	}

	std::vector<VoxelChunk*>* getChunks()
	{
		return &m_chunks;
	}

	VoxelChunk* findChunk(int x, int z)
	{
		return nullptr;
	}

	void findNeighs(VoxelChunk* chunk)
	{
		// u/l/d/r
		if(!chunk->m_neighN)
		{
			// find chunk
			chunk->m_neighN = findChunk(chunk->m_x, chunk->m_z + 1);
		}

		if (!chunk->m_neighE)
		{
			// find chunk
			chunk->m_neighE = findChunk(chunk->m_x + 1, chunk->m_z);
		}

		if (!chunk->m_neighS)
		{
			// find chunk
			chunk->m_neighS = findChunk(chunk->m_x, chunk->m_z - 1);
		}

		if (!chunk->m_neighW)
		{
			// find chunk
			chunk->m_neighW = findChunk(chunk->m_x - 1, chunk->m_z);
		}

		// corners
		if (!chunk->m_neighNE)
		{
			// find chunk
			chunk->m_neighNE = findChunk(chunk->m_x + 1, chunk->m_z + 1);
		}

		if (!chunk->m_neighSE)
		{
			// find chunk
			chunk->m_neighSE = findChunk(chunk->m_x + 1, chunk->m_z - 1);
		}

		if (!chunk->m_neighSW)
		{
			// find chunk
			chunk->m_neighSW = findChunk(chunk->m_x - 1, chunk->m_z - 1);
		}

		if (!chunk->m_neighNW)
		{
			// find chunk
			chunk->m_neighNW = findChunk(chunk->m_x - 1, chunk->m_z + 1);
		}
	}

	void dispose()
	{
		for (auto && root : m_roots)
		{
			root->dispose();
		}

		delete this;
	}
};

#endif // VOXELCHUNKTREE_H