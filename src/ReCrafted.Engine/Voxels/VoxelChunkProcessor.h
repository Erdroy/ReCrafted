// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef VOXELCHUNKPROCESSOR_H
#define VOXELCHUNKPROCESSOR_H

// includes
#include "VoxelChunk.h"

#include <mutex>
#include <thread>

/// <summary>
/// VoxelChunkProcessor class.
/// </summary>
class VoxelChunkProcessor
{
	friend class Universe;

public:
	/// <summary>
	/// QueueType enum.
	/// </summary>
	struct QueueType
	{
		enum Enum
		{
			VoxelData,
			VoxelDataAndMesh,

			Count
		};
	};

private:
	static VoxelChunkProcessor* m_instance;

private:
    Array<VoxelChunk*> m_dataQueue = {};
    Array<VoxelChunk*> m_meshingQueue = {};
	std::mutex m_dataQueueMutex = {};
	std::mutex m_meshingQueueMutex = {};

    Array<std::thread*> m_workers = {};

	int m_totalData = 0;
	int m_totalMesh = 0;

public:
	float m_dataTime = 0;
	float m_meshTime = 0;

private:
	VoxelChunkProcessor()
	{
		m_instance = this;
	}

private:
	void init();

public:
	/// <summary>
	/// Dispose VCP.
	/// </summary>
	void dispose();

	/// <summary>
	/// Find and dequeue chunk for voxel data generation.
	/// </summary>
	/// <returns>The found chunk or null when no any chunk needs voxel data generation.</returns>
	VoxelChunk* dequeueDataLessChunk();

	/// <summary>
	/// Find and dequeue chunk for meshing.
	/// </summary>
	/// <returns>The found chunk or null when no any chunk needs meshing.</returns>
	VoxelChunk* dequeueMeshLessChunk();

	/// <summary>
	/// Finish voxel data generation for specified chunk.
	/// </summary>
	/// <param name="chunk">The chunk.</param>
	void finishChunkData(VoxelChunk* chunk) const;

	/// <summary>
	/// Finish meshing for specified chunk.
	/// </summary>
	/// <param name="chunk">The chunk.</param>
	void finishChunkMesh(VoxelChunk* chunk) const;

public:
	/// <summary>
	/// Queue chunk for processing.
	/// </summary>
	/// <param name="chunk">The chunk.</param>
	FORCEINLINE static void queue(VoxelChunk* chunk, QueueType::Enum queueType, bool force = false)
	{
		if(queueType == QueueType::VoxelData) // try to add to voxel data queue
		{
			if (chunk->m_hasVoxels)
				return;

			chunk->m_queued = true;
			m_instance->m_dataQueueMutex.lock();
			m_instance->m_dataQueue.add(chunk);
			m_instance->m_dataQueueMutex.unlock();
		}
		else
		{
			if (!chunk->m_hasVoxels) // try to add to voxel data queue
			{
				chunk->m_queued = true;
				m_instance->m_dataQueueMutex.lock();
				m_instance->m_dataQueue.add(chunk);
				m_instance->m_dataQueueMutex.unlock();
			}

			if (!force) {
				if (!chunk->m_mesh && chunk->hasNeighs()) // try to add to voxel meshing queue
				{
					chunk->m_queued = true;
					m_instance->m_meshingQueueMutex.lock();
					m_instance->m_meshingQueue.add(chunk);
					m_instance->m_meshingQueueMutex.unlock();
				}
			}
			else
			{
				if (chunk->hasNeighs()) // try to add to voxel meshing queue
				{
					chunk->m_queued = true;
					m_instance->m_meshingQueueMutex.lock();
					m_instance->m_meshingQueue.add(chunk);
					m_instance->m_meshingQueueMutex.unlock();
				}
			}
		}
	}

	/// <summary>
	/// Gets chunk processing queued chunks count.
	/// </summary>
	/// <returns>The amount of chunks that are waiting for processing.</returns>
	FORCEINLINE static uint getQueueSize()
	{
		return uint(m_instance->m_dataQueue.size() + m_instance->m_meshingQueue.size());
	}
};

#endif // VOXELCHUNKPROCESSOR_H
