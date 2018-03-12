// ReCrafted (c) 2016-2018 Always Too Late

#include "Universe.h"
#include "Common/Input/Input.h"
#include "Graphics/Camera.h"
#include "Graphics/DebugDraw.h"
#include "World/SpaceObject/SpaceObjectManager.h"
#include "World/SpaceObject/Storage/VoxelStorage.h"

SINGLETON_IMPL(Universe)

bool m_viewUpdateEnabled = true;

void Universe::onInit()
{
    // initialize save system

    // initialize space object manager
    SpaceObjectManager::getInstance()->init();

    // temporary, replace with World::load("../saves/SaveName", MakeDelegate(Universe::OnWorldLoaded));
    // when saves will be done
    SpaceObjectSettings settings;
    settings.name = "moon";
    settings.fileName = "../assets/spacebodies/moon";
    settings.saveName = "../saves/dev/voxelstorage/moon.vxh";
    settings.generationType = GenerationType::CubeHeightMap;
    settings.position = Vector3::zero();
    settings.minSurfaceHeight = 900.0f;
    settings.maxSurfaceHeight = settings.minSurfaceHeight + 120.0f; // will round up to 1024 * 2
    settings.hillsHeight = 40.0f;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

    m_testObject1 = SpaceObject::createSpaceObject(settings);
}

void Universe::onDispose()
{
    // shutdown
    SafeDisposeNN(SpaceObjectManager::getInstance());
    SafeDispose(m_testObject1);
}

void Universe::update()
{
	SpaceObjectManager::getInstance()->update();

	if(Input::isKeyDown(Key_F7))
	{
		m_viewUpdateEnabled = !m_viewUpdateEnabled;
	}

    if(Input::isKeyDown(Key_Mouse0))
    {
        var modPosition = Camera::getMainCamera()->get_position() + Camera::getMainCamera()->get_forward() * 5.0f;
        m_testObject1->modify(VoxelEditMode::Subtractive, VoxelEditShape::Sphere, modPosition, 2.5f);
    }
    if (Input::isKeyDown(Key_Mouse2))
    {
        var modPosition = Camera::getMainCamera()->get_position() + Camera::getMainCamera()->get_forward() * 5.0f;
        m_testObject1->modify(VoxelEditMode::Subtractive, VoxelEditShape::Cube, modPosition, 2.5f);
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

void Universe::render()
{
    m_testObject1->draw();
}
