// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNKTREE_H
#define VOXELCHUNKTREE_H

// includes
#include <vector>
#include "VoxelChunk.h"
#include "../../Graphics/Camera.h"

const int DepthTable[6] = { 512, 256, 128, 64, 32, 16 };

class VoxelChunkTree
{
private:
	struct vctEntry
	{
	public:
		int depth = 0;

		int left = 0;
		int bottom = 0;
		int right = 0;
		int top = 0;

		vctEntry* entries[4] = { nullptr, nullptr, nullptr, nullptr };
		VoxelChunk* chunk = nullptr;

		bool intersects(int x, int z, int size) const
		{
			return left <= x && bottom <= z && right >= x + size && top >= z + size;
		}

		void dispose() const
		{
			
		}
	};

private:
	/* 0 - 16384, 1 - 4096, 2 - 1024, 3 - 256, 4 - 64, 5 - 16*/
	std::vector<vctEntry*> m_roots = {};

public:
	void add(VoxelChunk* chunk)
	{
		auto targetX = chunk->m_x * 16;
		auto targetZ = chunk->m_z * 16;

		vctEntry* root = nullptr;
		if(m_roots.size() == 0)
		{
			root = new vctEntry;

			// set coords
			root->left = -DepthTable[0] / 2;
			root->bottom = -DepthTable[0] / 2;
			root->right = DepthTable[0] / 2;
			root->top = DepthTable[0] / 2;

			m_roots.push_back(root);
		}
		else if(m_roots.size() == 1)
		{
			root = m_roots[0];
		}
		else
		{
			// TODO: find vct root
			_ASSERT(false);
		}

		// iterate
		for(auto depth = 1; depth < 6; depth ++)
		{
			// check if root has entries
			for(auto i = 0; i < 4; i ++)
			{
				// generate entries when root does not have any
				if (root->entries[i] == nullptr)
				{
					auto node = root->entries[i] = new vctEntry;
					node->depth = depth;

					auto depthSzOffset = DepthTable[depth];

					if(i == 0)
					{
						node->top = root->top;
						node->bottom = root->bottom + depthSzOffset;
						node->left = root->left;
						node->right = root->right - depthSzOffset;
					}
					if (i == 1)
					{
						node->top = root->top;
						node->bottom = root->bottom + depthSzOffset;
						node->left = root->left + depthSzOffset;
						node->right = root->right;
					}
					if (i == 2)
					{
						node->top = root->top - depthSzOffset;
						node->bottom = root->bottom;
						node->left = root->left + depthSzOffset;
						node->right = root->right;
					}
					if (i == 3)
					{
						node->top = root->top - depthSzOffset;
						node->bottom = root->bottom;
						node->left = root->left;
						node->right = root->right - depthSzOffset;
					}
				}

				if(root->entries[i]->intersects(targetX, targetZ, 16))
				{
					if(depth == 5)
					{
						root->entries[i]->chunk = chunk;
						return;
					}

					root = root->entries[i];
					break;
				}
			}
		}
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