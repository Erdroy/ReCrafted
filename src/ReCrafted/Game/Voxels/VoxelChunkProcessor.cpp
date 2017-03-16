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
				Sleep(5);
				continue;
			}
		}

		// process
		chunk->worker_dataGenerate();
		VoxelChunkProcessorInstance->finishChunkData(chunk);

		// try to set neightbours


		chunk = nullptr;

		// sleep 5ms if there is no any chunks left
		if(queue->size() == 0u)
		{
			Sleep(5);
		}
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
				Sleep(5);
				continue;
			}
		}

		// process
		chunk->worker_meshGenerate();
		VoxelChunkProcessorInstance->finishChunkMesh(chunk);
		chunk = nullptr;

		// sleep 5ms if there is no any chunks left
		if (queue->size() == 0u)
		{
			Sleep(5);
		}
	}
}

void VoxelChunkProcessor::init()
{
	VoxelChunkProcessorInstance = this;

	// create threads
	m_dataThread = Platform::createThread(ThreadFunction(worker_data), &m_dataQueue);
	m_meshingThread = Platform::createThread(ThreadFunction(worker_meshing), &m_meshingQueue);

	Logger::write("Created threads for VoxelChunkProcessor", LogLevel::Info);

	// TODO: multi threading
}

void VoxelChunkProcessor::dispose()
{
	Platform::killThread(&m_dataThread);
	Platform::killThread(&m_meshingThread);
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

	return nullptr;
}

void VoxelChunkProcessor::finishChunkData(VoxelChunk* chunk)
{
	chunk->m_processing = false;
	chunk->m_hasVoxels = true;
}

void VoxelChunkProcessor::finishChunkMesh(VoxelChunk* chunk)
{
	chunk->m_processing = false;
}
