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
	m_currentWorld->draw();
}
