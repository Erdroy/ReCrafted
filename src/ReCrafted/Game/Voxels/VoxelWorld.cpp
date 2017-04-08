// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"
#include "VoxelChunkProcessor.h"
#include "../../Graphics/Camera.h"
#include "../../Core/Input.h"
#include <string>

void VoxelWorld::update_digplace()
{
	auto dig = Input::isKeyDown(Key_Mouse0);
	//auto place = Input::isKeyDown(Key_Mouse1);

	if(dig)
	{
		// cast ray
		RaycastHit hit;
		if(raycast(Camera::getMainCamera()->get_position(), Camera::getMainCamera()->get_forward(), 7.0f, &hit))
		{
			// hit!
			auto chunk = getVoxelChunk(hit.position);
			chunk->setVoxel(0, hit.position);

			// rebuild chunk
			chunk->updateMesh();

			// check if the block was on the edge, if so, also rebuild the needed neighs
		}
	}
}

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

	// update block-place/dig
	update_digplace();
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
