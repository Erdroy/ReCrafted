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
		/// Gets chunk at given index.
		/// </summary>
		/// <param name="x">The index</param>
		/// <returns>The chunk.</returns>
		VoxelChunk* getChunk(int index)
		{
			return chunkTable[index];
		}

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
		const auto wsSize = float(tableWidth * ChunkWidth);
		auto sqrDistance = distance * distance;

		for(auto root : m_roots)
		{
			auto wsX = float(root->x * wsSize);
			auto wsZ = float(root->z * wsSize);

			auto deltaX = point.X - Math::maxf(wsX, Math::minf(point.X, wsX + wsSize));
			auto deltaY = point.Y - Math::maxf(wsZ, Math::minf(point.X, wsZ + wsSize));

			auto isinrange = deltaX * deltaX + deltaY * deltaY < sqrDistance;
			auto isinside = point.X > wsX && point.X < wsX + wsSize && point.Y > wsZ && point.Y < wsZ + wsSize;

			if(isinrange || isinside)
			{
				// check chunks
				for(auto i = 0; i < tableWidth * tableWidth; i ++) // TODO: optimize this loop, skip x axes that are out of range
				{
					auto chunk = root->getChunk(i);
					
					if (!chunk)
						continue;

					wsX = float(chunk->m_x * ChunkWidth);
					wsZ = float(chunk->m_z * ChunkWidth);

					deltaX = point.X - Math::maxf(wsX, Math::minf(point.X, wsX + ChunkWidth));
					deltaY = point.Y - Math::maxf(wsZ, Math::minf(point.X, wsZ + ChunkWidth));

					if (deltaX * deltaX + deltaY * deltaY < sqrDistance)
						chunks->push_back(chunk);
				}
			}
		}
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