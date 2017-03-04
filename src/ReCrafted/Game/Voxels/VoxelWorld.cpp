// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"
#include "../../Core/Profiler.h"

void VoxelWorld::init(bool generateworld)
{
	// initialize generator
	VoxelGenerator::initialize();

	m_chunkTree = new VoxelChunkTree;

	Profiler::beginProfile();
	if (generateworld)
	{
		Profiler::beginProfile();
		for(auto x = -32; x < 32; x ++)
		{
			for (auto z = -32; z < 32; z++)
			{
				generateChunk(x, z);
			}
		}

		Profiler::endProfile("Starting chunks voxel data generated in %0.7f ms.");

		m_visibleChunks.clear();
		m_chunkTree->getNearChunks(Vector2(0.0f, 0.0f), 250.0f, &m_visibleChunks);

		Profiler::beginProfile();
		for (auto chunk : m_visibleChunks)
		{
			chunk->meshGenerate();
		}
		Profiler::endProfile("Starting chunks meshes generated in %0.7f ms.");
	}
	Profiler::endProfile("Starting world generated in %0.7f ms.");
}

void VoxelWorld::update()
{
	auto camera = Camera::getMainCamera();

	m_visibleChunks.clear();
	m_chunkTree->getNearChunks(Vector2(camera->m_position.X, camera->m_position.Z), 250.0f, &m_visibleChunks);
	
	for (auto chunk : m_visibleChunks)
	{
		if(!chunk->m_mesh)
			chunk->meshGenerate();
	}

	for(auto chunk : *m_chunkTree->getChunks())
	{
		chunk->update();
	}
}

void VoxelWorld::simulate()
{
	for (auto chunk : *m_chunkTree->getChunks())
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
		chunk->m_neighN = m_chunkTree->findChunk(chunk->m_x, chunk->m_z + 1);
	}

	if (!chunk->m_neighE)
	{
		// find chunk
		chunk->m_neighE = m_chunkTree->findChunk(chunk->m_x + 1, chunk->m_z);
	}

	if (!chunk->m_neighS)
	{
		// find chunk
		chunk->m_neighS = m_chunkTree->findChunk(chunk->m_x, chunk->m_z - 1);
	}

	if (!chunk->m_neighW)
	{
		// find chunk
		chunk->m_neighW = m_chunkTree->findChunk(chunk->m_x - 1, chunk->m_z);
	}

	// corners
	if (!chunk->m_neighNE)
	{
		// find chunk
		chunk->m_neighNE = m_chunkTree->findChunk(chunk->m_x + 1, chunk->m_z + 1);
	}

	if (!chunk->m_neighSE)
	{
		// find chunk
		chunk->m_neighSE = m_chunkTree->findChunk(chunk->m_x + 1, chunk->m_z - 1);
	}

	if (!chunk->m_neighSW)
	{
		// find chunk
		chunk->m_neighSW = m_chunkTree->findChunk(chunk->m_x - 1, chunk->m_z - 1);
	}

	if (!chunk->m_neighNW)
	{
		// find chunk
		chunk->m_neighNW = m_chunkTree->findChunk(chunk->m_x - 1, chunk->m_z + 1);
	}
}

void VoxelWorld::generateNeigs(VoxelChunk* chunk)
{
	auto x = chunk->m_x;
	auto z = chunk->m_z;

	if (!chunk->m_neighN)
	{
		chunk->m_neighN = generateChunk(x, z + 1);
	}
	if (!chunk->m_neighE)
	{
		chunk->m_neighE = generateChunk(x + 1, z);
	}

	if (!chunk->m_neighS)
	{
		chunk->m_neighS = generateChunk(x, z - 1);
	}

	if (!chunk->m_neighW)
	{
		chunk->m_neighW = generateChunk(x - 1, z);
	}

	if (!chunk->m_neighNE)
	{
		chunk->m_neighNE = generateChunk(x + 1, z + 1);
	}

	if (!chunk->m_neighSE)
	{
		chunk->m_neighSE = generateChunk(x + 1, z - 1);
	}

	if (!chunk->m_neighSW)
	{
		chunk->m_neighSW = generateChunk(x - 1, z - 1);
	}

	if (!chunk->m_neighNW)
	{
		chunk->m_neighNW = generateChunk(x - 1, z + 1);
	}
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
