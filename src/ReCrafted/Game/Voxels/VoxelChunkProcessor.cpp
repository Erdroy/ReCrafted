// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelChunkProcessor.h"
#include "../../Core/GameBase.h"
#include "../../Core/Logger.h"
#include "../../Graphics/Camera.h"

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
				Sleep(5);
				continue;
			}
		}

		// process
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
			chunk->m_processing = true;
			chunk->m_queued = false;

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
	VoxelChunk* tempchunk;
	VoxelChunk* chunk = nullptr;

	auto cameraPos = Camera::getMainCamera()->get_position();

	m_meshingQueueMutex.lock();
	for (auto i = 0u; i < m_meshingQueue.size(); i++)
	{
		tempchunk = m_meshingQueue[i];

		// search
		if (!tempchunk->m_processing && tempchunk->m_hasVoxels)
		{
			// dequeue chunk if it is out of view range
			auto rX = tempchunk->m_x * ChunkWidth - cameraPos.x;
			auto rZ = tempchunk->m_z * ChunkWidth - cameraPos.z;
			auto dist = sqrt(rX * rX + rZ * rZ);
			if (dist > 500.0f)
			{
				tempchunk->m_queued = false;
				m_meshingQueue.erase(m_meshingQueue.begin() + i);
				continue;
			}

			if(tempchunk->hasNeighs() && tempchunk->hasLoadedNeighs())
			{
				chunk = tempchunk;
				chunk->m_processing = true;
				chunk->m_queued = false;

				// remove chunk
				m_meshingQueue.erase(m_meshingQueue.begin() + i);
				break;
			}
		}
	}

	m_meshingQueueMutex.unlock();

	return chunk;
}

void VoxelChunkProcessor::finishChunkData(VoxelChunk* chunk) const
{
	m_instance->m_totalData++;
	chunk->m_processing = false;
	chunk->m_hasVoxels = true;
	chunk->m_queued = false;
}

void VoxelChunkProcessor::finishChunkMesh(VoxelChunk* chunk) const
{
	m_instance->m_totalMesh++;
	chunk->m_processing = false;
	chunk->m_queued = false;
}
