// ReCrafted © 2016-2017 Always Too Late

#include "Universe.h"
#include "Common/Time.h"
#include "Core/Logger.h"
#include "Graphics/Rendering.h"
#include "Graphics/Camera.h"
#include "World/MarchingCubes.h"
#include "Graphics/DebugDraw.h"

Universe* Universe::m_instance;
//MarchingCubes mc;

void Universe::init()
{
	m_testObject = SpaceObject::createSpaceObject();
	m_testObject->init();

	//mc = {};
	//mc.generate();
}

void Universe::update()
{
	auto cameraPosition = Camera::getMainCamera()->get_position();
	m_testObject->updateViewPoint(cameraPosition);
	m_testObject->update();
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

	m_testObject->draw();
	//mc.draw();
}

void Universe::dispose()
{
	m_testObject->dispose();

	Logger::write("Universe unloaded", LogLevel::Info);
	delete this;
}
