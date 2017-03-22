// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef VOXELCHUNKPROCESSOR_H
#define VOXELCHUNKPROCESSOR_H

// includes
#include "VoxelChunk.h"
#include "../../Platform/Platform.Windows.h"
#include <mutex>


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
	std::vector<VoxelChunk*> m_dataQueue = {};
	std::vector<VoxelChunk*> m_meshingQueue = {};
	std::mutex m_dataQueueMutex = {};
	std::mutex m_meshingQueueMutex = {};

	std::vector<Thread> m_workers = {};

	int m_total = 0;

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

	VoxelChunk* dequeueDataLessChunk();
	VoxelChunk* dequeueMeshLessChunk();

	void finishChunkData(VoxelChunk* chunk) const;
	void finishChunkMesh(VoxelChunk* chunk) const;

public:
	/// <summary>
	/// Queue chunk for processing.
	/// </summary>
	/// <param name="chunk">The chunk.</param>
	FORCEINLINE static void queue(VoxelChunk* chunk, QueueType::Enum queueType)
	{
		if(queueType == QueueType::VoxelData) // try to add to voxel data queue
		{
			if (chunk->m_hasVoxels)
				return;

			m_instance->m_total++;
			chunk->m_queued = true;
			m_instance->m_dataQueueMutex.lock();
			m_instance->m_dataQueue.push_back(chunk);
			m_instance->m_dataQueueMutex.unlock();
		}
		else
		{
			if (!chunk->m_hasVoxels) // try to add to voxel data queue
			{
				m_instance->m_total++;
				chunk->m_queued = true;
				m_instance->m_dataQueueMutex.lock();
				m_instance->m_dataQueue.push_back(chunk);
				m_instance->m_dataQueueMutex.unlock();
			}

			if(!chunk->m_mesh) // try to add to voxel meshing queue
			{
				m_instance->m_total++;
				chunk->m_queued = true;
				m_instance->m_meshingQueueMutex.lock();
				m_instance->m_meshingQueue.push_back(chunk);
				m_instance->m_meshingQueueMutex.unlock();
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
