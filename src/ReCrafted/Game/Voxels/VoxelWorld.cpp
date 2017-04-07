// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"
#include "../../Graphics/Camera.h"
#include "VoxelChunkProcessor.h"
#include "../../Core/Input.h"
#include <string>

void VoxelWorld::init()
{
	// initialize generator
	VoxelGenerator::initialize();

	// create voxel grid
	m_chunkMap = new VoxelMap;
}

void VoxelWorld::update()
{
	for(auto chunk : *m_chunkMap->getChunks())
	{
		chunk->update();
	}
}

void VoxelWorld::simulate()
{
	for (auto chunk : *m_chunkMap->getChunks())
	{
		// TODO: check if chunk should be simulated
		chunk->simulate();
	}
}

void VoxelWorld::draw()
{
	m_chunkMap->draw();
}

VoxelChunk* VoxelWorld::generateChunk(int x, int z)
{
	auto chunk = new VoxelChunk;
	chunk->world = this;
	chunk->m_x = x;
	chunk->m_z = z;

	// add to chunk map
	m_chunkMap->addChunk(chunk);

	// queue chunk for generation
	VoxelChunkProcessor::queue(chunk, VoxelChunkProcessor::QueueType::VoxelData);

	return chunk;
}

VoxelChunk* VoxelWorld::getVoxelChunk(Vector3 containedPoint) const
{
	return m_chunkMap->findChunk(static_cast<int>(containedPoint.x) / 16, static_cast<int>(containedPoint.z) / 16);
}

bool VoxelWorld::raycast(Vector3 origin, Vector3 direction, float length, RaycastHit* hit)
{
	auto baseChunk = getVoxelChunk(origin);

	if (baseChunk == nullptr)
		return false;

	if (baseChunk->raycast(origin, direction, length, hit))
		return true;

	return false;
}
