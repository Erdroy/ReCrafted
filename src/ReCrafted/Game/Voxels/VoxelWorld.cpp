// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"
#include "Generator/VoxelGenerator.h"

void VoxelWorld::init(bool generateworld)
{
	// initialize generator
	VoxelGenerator::initialize();

	if (generateworld)
	{
		auto chunk = new VoxelChunk;
		chunk->world = this;
		chunk->m_x = 0;
		chunk->m_z = 0;

		chunk->dataGenerate();
		chunk->meshGenerate();

		m_chunks.push_back(chunk);

		chunk->neighN()->meshGenerate();
		chunk->neighE()->meshGenerate();
		chunk->neighS()->meshGenerate();
		chunk->neighW()->meshGenerate();

		chunk->neighE()->neighS()->meshGenerate();
		chunk->neighN()->neighW()->meshGenerate();
	}
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
