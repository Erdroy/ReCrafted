// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Universe.h"
#include "../Graphics/Rendering.h"
#include "../Graphics/Camera.h"
#include "../Core/Time.h"
#include "Items/ItemDB.h"

Universe* Universe::m_instance;

void loadAssets()
{
	Item granite = {};
	granite.itemType = ItemType::block;
	granite.name = TEXT("Granite");
	granite.description = TEXT("Granite, huh - stone stolen from minecraft.");
	granite.data = ItemDB::createBlockData(1, "../assets/textures/granite.png");

	ItemDB::registerItem(1, granite);

	Item dirt = {};
	dirt.itemType = ItemType::block;
	dirt.name = TEXT("Dirt");
	dirt.description = TEXT("Dirt, huh - dirt stolen from minecraft.");
	dirt.data = ItemDB::createBlockData(2, "../assets/textures/dirt.png");

	ItemDB::registerItem(2, dirt);

	Item planks = {};
	planks.itemType = ItemType::block;
	planks.name = TEXT("Planks");
	planks.description = TEXT("Planks, huh - oak planks stolen from minecraft.");
	planks.data = ItemDB::createBlockData(3, "../assets/textures/planks.png");

	ItemDB::registerItem(3, planks);
}

void Universe::init()
{
	ItemDB::init();

	loadAssets();

	ItemDB::generateAtlases();

	m_chunkProcessor.reset(new VoxelChunkProcessor);
	m_chunkProcessor->init();

	m_currentWorld.reset(new VoxelWorld);
	m_currentWorld->init();
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
	auto cameraPosition = Camera::getMainCamera()->get_position();

	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(1, 0, 0x4, "FPS: %.1f", 1.0f / Time::deltaTime());
	bgfx::dbgTextPrintf(1, 1, 0x4, "Delta time: %.5f", Time::deltaTime());
	bgfx::dbgTextPrintf(1, 2, 0x4, "Chunk voxel queue size: %d", m_chunkProcessor->m_dataQueue.size());
	bgfx::dbgTextPrintf(1, 3, 0x4, "Chunk meshing queue size: %d", m_chunkProcessor->m_meshingQueue.size());
	bgfx::dbgTextPrintf(1, 4, 0x4, "Total voxel queue size: %d", m_chunkProcessor->m_totalData);
	bgfx::dbgTextPrintf(1, 5, 0x4, "Total meshing queue size: %d", m_chunkProcessor->m_totalMesh);
	bgfx::dbgTextPrintf(1, 6, 0x4, "Chunk count: %d", m_currentWorld->m_chunkMap->getChunks()->size());
	// 7 - 'Drawn chunks'
	bgfx::dbgTextPrintf(1, 8, 0x4, "Camera position: { X: %.1f, Y: %.1f, Z: %.1f }", cameraPosition.x, cameraPosition.y, cameraPosition.z);


	m_currentWorld->draw();
}

void Universe::dispose()
{
	ItemDB::shutdown();

	m_currentWorld->dispose();
	m_chunkProcessor->dispose();
	Logger::write("Universe unloaded", LogLevel::Info);
	delete this;
}
