// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "VoxelWorld.h"

void VoxelWorld::init(bool generateworld)
{
	auto chunk = new VoxelChunk;
	chunk->dataGenerate();
	chunk->meshGenerate();

	m_chunks.push_back(chunk);
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
