// ReCrafted © 2016-2017 Always Too Late

#include "Universe.h"
#include "Common/Time.h"
#include "Common/Input/Input.h"
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

	SpaceObjectSettings settings;
    settings.name = "moon";
    settings.fileName = "../assets/spacebodies/moon";
    settings.generationType = GenerationType::CubeHeightMap;
    settings.position = Vector3::zero();
	settings.minSurfaceHeight = 900.0f;
	settings.maxSurfaceHeight = settings.minSurfaceHeight + 120.0f; // will round up to 1024 * 2
    settings.hillsHeight = 35.0f;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

	m_testObject1 = SpaceObject::createSpaceObject(settings);
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
	m_testObject1->draw();
}

void Universe::dispose()
{
	// shutdown
	SafeDisposeNN(SpaceObjectManager::getInstance());

    SafeDispose(m_testObject1);

	Logger::logInfo("Universe unloaded");
	delete this;
}
