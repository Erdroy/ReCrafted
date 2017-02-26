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
		/*for(auto x = -10; x < 10; x ++)
		{
			for (auto z = -10; z < 10; z++)
			{
				auto chunk = new VoxelChunk;
				chunk->world = this;
				chunk->m_x = x;
				chunk->m_z = z;
				
				m_chunkTree->add(chunk);
			}
		}*/

		auto chunk = new VoxelChunk;
		chunk->world = this;
		chunk->m_x = 0;
		chunk->m_z = 0;

		m_chunkTree->add(chunk);

		/*for (auto && chunk : m_chunks)
		{
			if ((chunk->m_x > -9 && chunk->m_x < 9) && (chunk->m_z > -9 && chunk->m_z < 9))
				chunk->meshGenerate();
		}*/
	}
	Profiler::endProfile("Starting world generated in %0.7f ms.");
}

void VoxelWorld::update()
{
	for(auto && chunk : m_chunks)
	{
		chunk->update();
	}
}

void VoxelWorld::simulate()
{
	for (auto && chunk : m_chunks)
	{
		// TODO: check if chunk should be simulated
		//chunk->simulate();
	}
}

void VoxelWorld::draw()
{
	for (auto && chunk : m_chunks)
	{
		// TODO: check if chunk is visible
		chunk->draw();
	}
}

void VoxelWorld::findNeighs(VoxelChunk* chunk)
{
	for (auto i = 0u; i < m_chunks.size(); i++)
	{
		if (!chunk->m_neighN)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x && 
				current->m_z == chunk->m_z + 1)
			{
				chunk->m_neighN = current;
			}
		}
		if (!chunk->m_neighNE)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x + 1 &&
				current->m_z == chunk->m_z + 1)
			{
				chunk->m_neighNE = current;
			}
		}
		if (!chunk->m_neighE)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x + 1 &&
				current->m_z == chunk->m_z)
			{
				chunk->m_neighE = current;
			}
		} 
		if (!chunk->m_neighSE)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x + 1 &&
				current->m_z == chunk->m_z - 1)
			{
				chunk->m_neighSE = current;
			}
		}
		if (!chunk->m_neighS)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x &&
				current->m_z == chunk->m_z - 1)
			{
				chunk->m_neighS = current;
			}
		}
		if (!chunk->m_neighSW)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x - 1 &&
				current->m_z == chunk->m_z - 1)
			{
				chunk->m_neighSW = current;
			}
		}
		if (!chunk->m_neighW)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x - 1 &&
				current->m_z == chunk->m_z)
			{
				chunk->m_neighW = current;
			}
		}
		if (!chunk->m_neighNW)
		{
			auto current = m_chunks[i];
			if (current->m_x == chunk->m_x - 1 &&
				current->m_z == chunk->m_z + 1)
			{
				chunk->m_neighNW = current;
			}
		}
	}
}

void VoxelWorld::initializeNeighs(VoxelChunk* chunk)
{
	if(!chunk->m_neighN)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x;
		neighchunk->m_z = chunk->m_z + 1;

		neighchunk->dataGenerate();
		// TODO: update neighs
		chunk->m_neighN = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighNE)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x + 1;
		neighchunk->m_z = chunk->m_z + 1;

		neighchunk->dataGenerate();

		chunk->m_neighNE = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighE)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x + 1;
		neighchunk->m_z = chunk->m_z;

		neighchunk->dataGenerate();

		chunk->m_neighE = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighSE)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x + 1;
		neighchunk->m_z = chunk->m_z - 1;

		neighchunk->dataGenerate();

		chunk->m_neighSE = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighS)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x;
		neighchunk->m_z = chunk->m_z - 1;

		neighchunk->dataGenerate();

		chunk->m_neighS = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighSW)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x - 1;
		neighchunk->m_z = chunk->m_z - 1;

		neighchunk->dataGenerate();

		chunk->m_neighSW = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighW)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x - 1;
		neighchunk->m_z = chunk->m_z;

		neighchunk->dataGenerate();

		chunk->m_neighW = neighchunk;
		m_chunks.push_back(neighchunk);
	}
	if (!chunk->m_neighNW)
	{
		auto neighchunk = new VoxelChunk;
		neighchunk->world = this;
		neighchunk->m_x = chunk->m_x - 1;
		neighchunk->m_z = chunk->m_z + 1;

		neighchunk->dataGenerate();

		chunk->m_neighNW = neighchunk;
		m_chunks.push_back(neighchunk);
	}
}

VoxelChunk* VoxelWorld::getVoxelChunk(Vector3 containedPoint)
{
	return nullptr;
}

bool VoxelWorld::raycast(Vector3 origin, Vector3 direction, voxelid* voxelid, Vector3* point, Vector3* normal)
{
	// http://www.cse.chalmers.se/edu/year/2010/course/TDA361/grid.pdf
}
