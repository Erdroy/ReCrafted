// ReCrafted © 2016-2017 Always Too Late

#include "Universe.h"
#include "Common/Time.h"
#include "Common/Input.h"
#include "Core/Logger.h"
#include "Graphics/Camera.h"
#include "Graphics/DebugDraw.h"
#include "World/SpaceObject/SpaceObjectManager.h"

Universe* Universe::m_instance;

bool m_viewUpdateEnabled = true;

void Universe::init()
{
	// initialize space object manager
	SpaceObjectManager::getInstance()->init();

	m_testObject1 = SpaceObject::createSpaceObject();
	m_testObject1->init(Vector3::zero(), 1024.0f);

	//m_testObject2 = SpaceObject::createSpaceObject();
	//m_testObject2->init(Vector3::one() * 300.0f, 128.0f);
}

void Universe::update()
{
	SpaceObjectManager::getInstance()->update();

	if(Input::isKeyDown(Key_F7))
	{
		m_viewUpdateEnabled = !m_viewUpdateEnabled;
	}

	if (m_viewUpdateEnabled) 
	{
		auto cameraPosition = Camera::getMainCamera()->get_position();

		m_testObject1->updateViewPoint(cameraPosition);
		m_testObject1->update();

		//m_testObject2->updateViewPoint(cameraPosition);
		//m_testObject2->update();
	}
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

	bgfx::dbgTextPrintf(1, 3, 0x4, "LoD Update %s", m_viewUpdateEnabled ? "enabled" : "disabled");

	m_testObject1->draw(); 
	//m_testObject2->draw();
}

void Universe::dispose()
{
	SafeDispose(m_testObject1);
	SafeDispose(m_testObject2);

	// shutdown
	SafeDisposeNN(SpaceObjectManager::getInstance());

	Logger::logInfo("Universe unloaded");
	delete this;
}
