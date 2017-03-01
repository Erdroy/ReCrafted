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
				auto chunk = new VoxelChunk;
				chunk->world = this;
				chunk->m_x = x;
				chunk->m_z = z;

				m_chunkTree->add(chunk);
			}
		}

		auto chunks = m_chunkTree->getChunks();
		for (auto i = 0; i < 64 * 64; i++)
		{
			(*chunks)[i]->dataGenerate();
		}
		Profiler::endProfile("Starting chunks voxel data generated in %0.7f ms.");

		m_visibleChunks.clear();
		m_chunkTree->getNearChunks(Vector2(0.0f, 0.0f), 100.0f, &m_visibleChunks);

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
	m_chunkTree->getNearChunks(Vector2(camera->m_position.X, camera->m_position.Z), 500.0f, &m_visibleChunks);
	
	for (auto chunk : m_visibleChunks)
	{
		if(!chunk->m_mesh)
			chunk->meshGenerate();

		chunk->update();
	}
}

void VoxelWorld::simulate()
{
	/*for (auto && chunk : m_chunks)
	{
		// TODO: check if chunk should be simulated
		chunk->simulate();
	}*/
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
	m_chunkTree->findNeighs(chunk);
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
