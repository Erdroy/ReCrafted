// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunkProcessor.h"
#include "../../Core/GameBase.h"
#include "../../Core/Logger.h"

VoxelChunkProcessor* VoxelChunkProcessor::m_instance;
VoxelChunkProcessor* VoxelChunkProcessorInstance;

void worker_data(std::vector<VoxelChunk*>* queue)
{
	VoxelChunk* chunk = nullptr;
	while(GameBase::isRunning())
	{
		if(!chunk)
		{
			// find new chunk
			chunk = VoxelChunkProcessorInstance->dequeueDataLessChunk();

			// if no chunk is found
			// sleep 5ms and continue
			if(chunk == nullptr)
			{
				Sleep(1);
				continue;
			}
		}

		// process
		chunk->worker_dataGenerate();
		VoxelChunkProcessorInstance->finishChunkData(chunk);

		// try to set neightbours
		chunk->updateNeighs();

		chunk = nullptr;
	}
}

void worker_meshing(std::vector<VoxelChunk*>* queue)
{
	VoxelChunk* chunk = nullptr;
	while (GameBase::isRunning())
	{
		if (!chunk)
		{
			// find new chunk
			chunk = VoxelChunkProcessorInstance->dequeueMeshLessChunk();

			// if no chunk is found
			// sleep 5ms and continue
			if (chunk == nullptr)
			{
				Sleep(1);
				continue;
			}
		}

		// process
		if (!chunk->hasNeighs())
			throw;

		chunk->worker_meshGenerate();
		VoxelChunkProcessorInstance->finishChunkMesh(chunk);

		chunk = nullptr;
	}
}

void VoxelChunkProcessor::init()
{
	VoxelChunkProcessorInstance = this;

	// create threads
	m_workers.push_back(Platform::createThread(ThreadFunction(worker_data), &m_dataQueue));
	m_workers.push_back(Platform::createThread(ThreadFunction(worker_meshing), &m_meshingQueue));

	Logger::write("Created threads for VoxelChunkProcessor", LogLevel::Info);

	// TODO: multi threading
}

void VoxelChunkProcessor::dispose()
{
	for(auto thread : m_workers)
		Platform::killThread(&thread);

	Logger::write("Killed VoxelChunkProcessor threads", LogLevel::Info);
}

VoxelChunk* VoxelChunkProcessor::dequeueDataLessChunk()
{
	VoxelChunk* chunk = nullptr;

	m_dataQueueMutex.lock();
	for(auto i = 0u; i < m_dataQueue.size(); i ++)
	{
		if(!m_dataQueue[i]->m_processing)
		{
			chunk = m_dataQueue[i];
			// remove chunk for dataQueue
			m_dataQueue.erase(m_dataQueue.begin() + i);
			break;
		}
	}
	m_dataQueueMutex.unlock();

	return chunk;
}

VoxelChunk* VoxelChunkProcessor::dequeueMeshLessChunk()
{
	// find chunk which has all neighbours
	VoxelChunk* chunk = nullptr;

	m_meshingQueueMutex.lock();
	for (auto i = 0u; i < m_meshingQueue.size(); i++)
	{
		if (!m_meshingQueue[i]->m_processing && m_meshingQueue[i]->m_hasVoxels)
		{
			if(m_meshingQueue[i]->hasNeighs())
			{
				chunk = m_meshingQueue[i];

				// remove chunk
				m_meshingQueue.erase(m_meshingQueue.begin() + i);
				break;
			}

			// remove chunk
			m_meshingQueue.erase(m_meshingQueue.begin() + i);
			break;
		}
	}
	m_meshingQueueMutex.unlock();

	return chunk;
}

void VoxelChunkProcessor::finishChunkData(VoxelChunk* chunk) const
{
	chunk->m_processing = false;
	chunk->m_hasVoxels = true;
}

void VoxelChunkProcessor::finishChunkMesh(VoxelChunk* chunk) const
{
	chunk->m_processing = false;
}
