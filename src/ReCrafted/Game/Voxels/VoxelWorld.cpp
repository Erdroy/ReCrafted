// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"
#include "../../Graphics/Camera.h"
#include "VoxelChunkProcessor.h"

void VoxelWorld::init(bool generateworld)
{
	// initialize generator
	VoxelGenerator::initialize();

	m_chunkMap = new VoxelChunkMap;
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

void VoxelWorld::findNeighs(VoxelChunk* chunk)
{
	if (!chunk->m_neighN)
	{
		// find chunk
		chunk->m_neighN = m_chunkMap->findChunk(chunk->m_x, chunk->m_z + 1);

		if (chunk->m_neighN)
			chunk->m_neighN->m_neighS = chunk;
	}

	if (!chunk->m_neighE)
	{
		// find chunk
		chunk->m_neighE = m_chunkMap->findChunk(chunk->m_x + 1, chunk->m_z);

		if (chunk->m_neighE)
			chunk->m_neighE->m_neighW = chunk;
	}

	if (!chunk->m_neighS)
	{
		// find chunk
		chunk->m_neighS = m_chunkMap->findChunk(chunk->m_x, chunk->m_z - 1);

		if (chunk->m_neighS)
			chunk->m_neighS->m_neighN = chunk;
	}

	if (!chunk->m_neighW)
	{
		// find chunk
		chunk->m_neighW = m_chunkMap->findChunk(chunk->m_x - 1, chunk->m_z);

		if (chunk->m_neighW)
			chunk->m_neighW->m_neighE = chunk;
	}

	// corners
	if (!chunk->m_neighNE)
	{
		// find chunk
		chunk->m_neighNE = m_chunkMap->findChunk(chunk->m_x + 1, chunk->m_z + 1);

		if (chunk->m_neighNE)
			chunk->m_neighNE->m_neighSW = chunk;
	}

	if (!chunk->m_neighSE)
	{
		// find chunk
		chunk->m_neighSE = m_chunkMap->findChunk(chunk->m_x + 1, chunk->m_z - 1);

		if (chunk->m_neighSE)
			chunk->m_neighSE->m_neighNW = chunk;
	}

	if (!chunk->m_neighSW)
	{
		// find chunk
		chunk->m_neighSW = m_chunkMap->findChunk(chunk->m_x - 1, chunk->m_z - 1);

		if (chunk->m_neighSW)
			chunk->m_neighSW->m_neighNE = chunk;
	}

	if (!chunk->m_neighNW)
	{
		// find chunk
		chunk->m_neighNW = m_chunkMap->findChunk(chunk->m_x - 1, chunk->m_z + 1);

		if (chunk->m_neighNW)
			chunk->m_neighNW->m_neighSE = chunk;
	}
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
	VoxelChunkProcessor::queue(chunk, VoxelChunkProcessor::QueueType::VoxelDataAndMesh);

	return chunk;
}

VoxelChunk* VoxelWorld::getVoxelChunk(Vector3 containedPoint)
{
	return nullptr;
}

VoxelChunk* VoxelWorld::getVoxelChunk(Vector3i position)
{
	return nullptr;
}

bool VoxelWorld::raycast(Vector3 origin, Vector3 direction, voxelid* voxelid, Vector3* point, Vector3* normal)
{
	// http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
	return false;
}
