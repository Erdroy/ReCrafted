// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"
#include "../../Core/Profiler.h"
#include "../../Graphics/Camera.h"

void VoxelWorld::init(bool generateworld)
{
	// initialize generator
	VoxelGenerator::initialize();

	m_chunkMap = new VoxelChunkMap;

	Profiler::beginProfile();
	if (generateworld)
	{
		for (auto x = -16; x < 16; x++)
		{
			for (auto z = -16; z < 16; z++)
			{
				generateChunk(x, z);
			}
		}
	}
	m_chunkMap->getVisibleChunks(Vector2::zero(), 250.0f, &m_visibleChunks);
	Profiler::endProfile("Starting world generated in %0.7f ms.");
}

void VoxelWorld::update()
{
	auto camera = Camera::getMainCamera();

	//m_visibleChunks.clear();
#ifdef _DEBUG
	//m_chunkMap->getVisibleChunks(Vector2(camera->m_position.X, camera->m_position.Z), 250.0f, &m_visibleChunks);
#else
	//m_chunkMap->getVisibleChunks(Vector2(camera->m_position.X, camera->m_position.Z), 250.0f, &m_visibleChunks);
#endif
	// TODO: better getNearChunks method
	
	for (auto chunk : m_visibleChunks)
	{
		if(!chunk->m_mesh)
			chunk->meshGenerate();
	}

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
	for (auto chunk : m_visibleChunks)
	{
		// TODO: check if chunk is visible
		chunk->draw();
	}
}

void VoxelWorld::findNeighs(VoxelChunk* chunk)
{
	if (!chunk->m_neighN)
	{
		// find chunk
		chunk->m_neighN = m_chunkMap->findChunk(chunk->m_x, chunk->m_z + 1);
	}

	if (!chunk->m_neighE)
	{
		// find chunk
		chunk->m_neighE = m_chunkMap->findChunk(chunk->m_x + 1, chunk->m_z);
	}

	if (!chunk->m_neighS)
	{
		// find chunk
		chunk->m_neighS = m_chunkMap->findChunk(chunk->m_x, chunk->m_z - 1);
	}

	if (!chunk->m_neighW)
	{
		// find chunk
		chunk->m_neighW = m_chunkMap->findChunk(chunk->m_x - 1, chunk->m_z);
	}

	// corners
	if (!chunk->m_neighNE)
	{
		// find chunk
		chunk->m_neighNE = m_chunkMap->findChunk(chunk->m_x + 1, chunk->m_z + 1);
	}

	if (!chunk->m_neighSE)
	{
		// find chunk
		chunk->m_neighSE = m_chunkMap->findChunk(chunk->m_x + 1, chunk->m_z - 1);
	}

	if (!chunk->m_neighSW)
	{
		// find chunk
		chunk->m_neighSW = m_chunkMap->findChunk(chunk->m_x - 1, chunk->m_z - 1);
	}

	if (!chunk->m_neighNW)
	{
		// find chunk
		chunk->m_neighNW = m_chunkMap->findChunk(chunk->m_x - 1, chunk->m_z + 1);
	}
}

void VoxelWorld::generateNeigs(VoxelChunk* chunk)
{
	auto x = chunk->m_x;
	auto z = chunk->m_z;

	if (!chunk->m_neighN)
	{
		auto newChunk = chunk->m_neighN = generateChunk(x, z + 1);
		findNeighs(newChunk);
	}
	if (!chunk->m_neighE)
	{
		auto newChunk = chunk->m_neighE = generateChunk(x + 1, z);
		findNeighs(newChunk);
	}

	if (!chunk->m_neighS)
	{
		auto newChunk = chunk->m_neighS = generateChunk(x, z - 1);
		findNeighs(newChunk);
	}

	if (!chunk->m_neighW)
	{
		auto newChunk = chunk->m_neighW = generateChunk(x - 1, z);
		findNeighs(newChunk);
	}

	if (!chunk->m_neighNE)
	{
		auto newChunk = chunk->m_neighNE = generateChunk(x + 1, z + 1);
		findNeighs(newChunk);
	}

	if (!chunk->m_neighSE)
	{
		auto newChunk = chunk->m_neighSE = generateChunk(x + 1, z - 1);
		findNeighs(newChunk);
	}

	if (!chunk->m_neighSW)
	{
		auto newChunk = chunk->m_neighSW = generateChunk(x - 1, z - 1);
		findNeighs(newChunk);
	}

	if (!chunk->m_neighNW)
	{
		auto newChunk = chunk->m_neighNW = generateChunk(x - 1, z + 1);
		findNeighs(newChunk);
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

	// generate data
	chunk->dataGenerate();

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
