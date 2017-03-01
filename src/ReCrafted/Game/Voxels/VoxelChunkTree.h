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
	private:
		VoxelChunk* chunkTable[tableWidth * tableWidth] = {};

	private:
		int getChunkIndex(int x, int z) const
		{
			auto rsx = x - this->x * tableWidth;
			auto rsz = z - this->z * tableWidth;

			return rsz * tableWidth + rsx;
		}

	public:
		// root/vct space
		int x = 0;
		int z = 0;

		/// <summary>
		/// Gets chunk at given location - chunk-space.
		/// </summary>
		/// <param name="x">X coord in chunk-space.</param>
		/// <param name="z">Z coord in chunk-space.</param>
		/// <returns>The chunk.</returns>
		VoxelChunk* getChunk(int x, int z)
		{
			auto index = getChunkIndex(x, z);
			return chunkTable[index];
		}

		/// <summary>
		/// Add new chunk to this entry.
		/// </summary>
		/// <param name="chunk">The new chunk.</param>
		void add(VoxelChunk* chunk)
		{
			auto index = getChunkIndex(chunk->m_x, chunk->m_z);
			chunkTable[index] = chunk;
		}

		/// <summary>
		/// Dispose the entry and all chunks.
		/// </summary>
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
	/// <summary>
	/// Add new chunk to the tree.
	/// </summary>
	/// <param name="chunk">The new chunk.</param>
	void add(VoxelChunk* chunk)
	{
		m_chunks.push_back(chunk);

		auto rootX = (chunk->m_x + 1 - tableWidth) / tableWidth;
		auto rootZ = (chunk->m_z + 1 - tableWidth) / tableWidth;

		vctEntry* root = nullptr;
		for(auto current : m_roots)
		{
			if(current->x == rootX && current->z == rootZ)
			{
				root = current;
				break;
			}
		}

		if(!root)
		{
			root = new vctEntry();

			root->x = rootX;
			root->z = rootZ;

			m_roots.push_back(root);
		}

		root->add(chunk);
	}

	/// <summary>
	/// Gets chunks that are near the point within given distance.
	/// </summary>
	/// <param name="point">The point.</param>
	/// <param name="distance">Distance.</param>
	/// <param name="chunks">The list to-which all found chunks will be added.</param>
	void getNearChunks(Vector2 point, float distance, std::vector<VoxelChunk*>* chunks)
	{
	}
	
	/// <summary>
	/// Gets chunks that are near the point within given distance using camera frustum to cull.
	/// </summary>
	/// <param name="point">The point.</param>
	/// <param name="distance">Distance.</param>
	/// <param name="camera">The camera that will be used to cull chunks using it's frustum.</param>
	/// <param name="chunks">The list to-which all found chunks will be added.</param>
	void getNearChunksCulled(Vector3 point, float distance, Camera* camera, std::vector<VoxelChunk*>* chunks)
	{

	}

	/// <summary>
	/// Gets all chunk list pointer.
	/// </summary>
	/// <returns>The chunk list pointer.</returns>
	std::vector<VoxelChunk*>* getChunks()
	{
		return &m_chunks;
	}

	/// <summary>
	/// Finds chunk at given coords.
	/// </summary>
	/// <param name="x">X coord in chunk-space.</param>
	/// <param name="z">Z coord in chunk-space.</param>
	/// <returns>The chunk.</returns>
	VoxelChunk* findChunk(int x, int z)
	{
		auto rootX = (x + 1 - tableWidth) / tableWidth;
		auto rootZ = (z + 1 - tableWidth) / tableWidth;

		vctEntry* root = nullptr;
		for (auto current : m_roots) // TODO: second and thirt upper levels that store roots - quad tree
		{
			if (current->x == rootX && current->z == rootZ)
			{
				root = current;
				break;
			}
		}

		if(!root)
			return nullptr;

		return root->getChunk(x, z);
	}

	/// <summary>
	/// Finds all neighs for chunk.
	/// </summary>
	/// <param name="chunk">The chunk which will get updated neighs.</param>
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

	/// <summary>
	/// Dispose the whole tree, dispose all entries and chunks.
	/// </summary>
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