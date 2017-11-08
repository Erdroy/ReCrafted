// ReCrafted © 2016-2017 Always Too Late

#include "Universe.h"
#include "Common/Time.h"
#include "Core/Logger.h"
#include "Graphics/Rendering.h"
#include "Graphics/Camera.h"
#include "Items/ItemDB.h"
#include "World/MarchingCubes.h"
#include "Graphics/DebugDraw.h"
#include "Core/Math/Color.h"

Universe* Universe::m_instance;
MarchingCubes mc;

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

	mc = {};
	mc.generate();
}

void Universe::update()
{
}

void Universe::simulate()
{
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
	bgfx::dbgTextPrintf(1, 2, 0x4, "Camera position: { X: %.1f, Y: %.1f, Z: %.1f }", cameraPosition.x, cameraPosition.y, cameraPosition.z);

	mc.draw();
}

void Universe::dispose()
{
	ItemDB::shutdown();

	Logger::write("Universe unloaded", LogLevel::Info);
	delete this;
}
