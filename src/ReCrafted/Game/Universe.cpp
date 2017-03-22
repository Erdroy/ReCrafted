// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Universe.h"
#include "../Graphics/Rendering.h"

Universe* Universe::m_instance;

void Universe::init()
{
	m_chunkProcessor.reset(new VoxelChunkProcessor);
	m_chunkProcessor->init();

	m_currentWorld.reset(new VoxelWorld);
	m_currentWorld->init(true);
}

void Universe::update()
{
	m_currentWorld->update();
}

void Universe::simulate()
{
	m_currentWorld->simulate();
}

void Universe::drawShadowCasters()
{

}

void Universe::draw()
{
	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(1, 1, 0x4, "Chunk voxeldata queue size: %d", m_chunkProcessor->m_dataQueue.size());
	bgfx::dbgTextPrintf(1, 2, 0x4, "Chunk meshing queue size: %d", m_chunkProcessor->m_meshingQueue.size());
	bgfx::dbgTextPrintf(1, 3, 0x4, "Chunk meshing queue size: %d", m_chunkProcessor->m_total);

	m_currentWorld->draw();
}
