// ReCrafted © 2016-2017 Always Too Late

#include "VoxelChunkProcessor.h"
#include "Core/GameMain.h"
#include "Core/Logger.h"
#include "Graphics/Camera.h"


VoxelChunkProcessor* VoxelChunkProcessor::m_instance;
VoxelChunkProcessor* VoxelChunkProcessorInstance;

void worker_data(Array<VoxelChunk*>* queue)
{
	VoxelChunk* chunk = nullptr;
	while(GameMain::isRunning())
	{
		if(!chunk)
		{
			// find new chunk
			chunk = VoxelChunkProcessorInstance->dequeueDataLessChunk();

			// if no chunk is found
			// sleep 5ms and continue
			if(chunk == nullptr)
			{
				Sleep(5); // TODO: this should be configurable
				continue;
			}
		}

		auto start = Platform::getMiliseconds();

		// process
		chunk->generateVoxelData();
		VoxelChunkProcessorInstance->finishChunkData(chunk);

		// try to set neightbours
		chunk->updateNeighs();

		chunk = nullptr;

		auto end = Platform::getMiliseconds();
		auto diff = end - start;
		VoxelChunkProcessorInstance->m_dataTime += float(diff);
	}
}

void worker_meshing(Array<VoxelChunk*>* queue)
{
	VoxelChunk* chunk = nullptr;

	// arrays for mesher
    Array<Vector3> verticesArray(65535);
    Array<Vector3> normalsArray(65535);
    Array<Vector2> uvsArray(65535);
    Array<Vector4> colorsArray(65535);
    Array<uint> indicesArray(65535);

	auto vertices_ptr = &verticesArray;
	auto normals_ptr = &normalsArray;
	auto uvs_ptr = &uvsArray;
	auto colors_ptr = &colorsArray;
	auto indices_ptr = &indicesArray;

	while (GameMain::isRunning())
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

		auto start = Platform::getMiliseconds();

		// process
		chunk->generateMesh(vertices_ptr, normals_ptr, uvs_ptr, colors_ptr, indices_ptr);
		VoxelChunkProcessorInstance->finishChunkMesh(chunk);

		// clear arrays
		verticesArray.clear();
		normalsArray.clear();
		uvsArray.clear();
		colorsArray.clear();
		indicesArray.clear();

		// zeroe chunk pointer
		chunk = nullptr;

		auto end = Platform::getMiliseconds();
		auto diff = end - start;
		VoxelChunkProcessorInstance->m_meshTime += float(diff);
	}

	// all vectors will release the memory
}

void VoxelChunkProcessor::init()
{
	VoxelChunkProcessorInstance = this;

	// create threads
	m_workers.add(new std::thread([=] { worker_data(&m_dataQueue); }));
	m_workers.add(new std::thread([=] { worker_meshing(&m_meshingQueue); }));

	Logger::write("Created threads for VoxelChunkProcessor", LogLevel::Info);

	// TODO: multi threading
}

void VoxelChunkProcessor::dispose()
{
	for(auto thread : m_workers)
	{
		if (thread->joinable())
			thread->join();

		SafeDelete(thread);
	}

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
			m_dataQueue.removeAt(i);
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
				m_meshingQueue.removeAt(i);
				continue;
			}

			if(tempchunk->hasNeighs() && tempchunk->hasLoadedNeighs())
			{
				chunk = tempchunk;
				chunk->m_processing = true;
				chunk->m_queued = false;

				// remove chunk
				m_meshingQueue.removeAt(i);
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
