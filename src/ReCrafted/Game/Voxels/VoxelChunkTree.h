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

private:
	struct vctEntry
	{
	public:
		int depth = 0;

		int x = 0;
		int z = 0;
		
		int startX = 0;
		int startZ = 0;
		int endX = 0;
		int endZ = 0;

		VoxelChunk* chunkTable[tableWidth * tableWidth] = {};

		void dispose() const
		{
			
			// suicide
			delete this;
		}

		bool contains(VoxelChunk* chunk) const
		{
			auto targetX = chunk->m_x * ChunkWidth;
			auto targetZ = chunk->m_z * ChunkWidth;

			return startX <= targetX && startZ <= targetZ && endX > targetX + ChunkWidth && endZ > targetZ + ChunkWidth;
		}
	};
private:
	std::vector<vctEntry*> m_roots = {};

public:
	void add(VoxelChunk* chunk)
	{
		vctEntry* root = nullptr;
		if(m_roots.size() == 0)
		{
			root = new vctEntry;

			// set coords
			root->x = 0;
			root->z = 0;

			auto worldSize = tableWidth * ChunkWidth;

			root->startX = root->x * worldSize - worldSize / 2;
			root->startZ = root->z * worldSize - worldSize / 2;
			root->endX = root->x * worldSize + worldSize / 2;
			root->endZ = root->z * worldSize + worldSize / 2;

			m_roots.push_back(root);
		}
		else if(m_roots.size() == 1)
		{
			root = m_roots[0];
		}
		else
		{
			// find vct root
			for (auto && current : m_roots)
			{
				// test simple rect intersection
				if (current->contains(chunk))
				{
					// this is the root of this chunk.
					root = current;
					break;
				}
			}

			// when root not found
			// create new one that will hold current chunk
			if(root == nullptr)
			{
				MISSING_CODE(); // TODO: create new root for current chunk
			}
		}

		// calculate root-space coords then index
		auto sz = tableWidth / 2;

		auto targetX = chunk->m_x + root->x + sz;
		auto targetZ = chunk->m_z + root->z + sz;

		auto index = targetZ * tableWidth + targetX;

		// insert into root
		root->chunkTable[index] = chunk;

		// done!
	}

	void getNearChunks(Vector3 point, float distance)
	{
		
	}

	void getNearChunksCulled(Vector3 point, float distance, Camera* camera)
	{

	}

	void dispose()
	{
		
	}
};

#endif // VOXELCHUNKTREE_H