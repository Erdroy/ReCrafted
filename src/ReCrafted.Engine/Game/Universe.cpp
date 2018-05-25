// ReCrafted (c) 2016-2018 Always Too Late

#include "Universe.h"
#include "Common/Input/Input.h"
#include "Graphics/Camera.h"
#include "World/SpaceObject/SpaceObjectManager.h"
#include "World/SpaceObject/Storage/VoxelStorage.h"

SINGLETON_IMPL(Universe)

bool m_viewUpdateEnabled = true;

void Universe::OnInit()
{
    // initialize save system

    // initialize space object manager
    SpaceObjectManager::GetInstance()->Init();

    // temporary, replace with World::load("../saves/SaveName", MakeDelegate(Universe::OnWorldLoaded));
    // when saves will be done
    SpaceObjectSettings settings;
    settings.name = "moon";
    settings.fileName = "../assets/spacebodies/moon";
    settings.saveName = "../saves/dev/voxelstorage/moon.vxh";
    settings.generationType = GenerationType::CubeHeightMap;
    settings.position = Vector3::Zero();
    settings.minSurfaceHeight = 900.0f;
    settings.maxSurfaceHeight = settings.minSurfaceHeight + 120.0f; // will round up to 1024 * 2
    settings.hillsHeight = 40.0f;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

    m_testObject1 = SpaceObject::CreateSpaceObject(settings);
}

void Universe::OnDispose()
{
    // Shutdown
    SafeDisposeNN(SpaceObjectManager::GetInstance());
    SafeDispose(m_testObject1);
}

void Universe::Update()
{
    SpaceObjectManager::GetInstance()->Update();

    if (Input::IsKeyDown(Key_F7))
    {
        m_viewUpdateEnabled = !m_viewUpdateEnabled;
    }

    var modPosition = Camera::GetMainCamera()->GetPosition() + Camera::GetMainCamera()->GetForward() * 5.0f;

    if (Input::IsKey(Key_Mouse0))
    {
        m_testObject1->Modify(VoxelEditMode::Subtractive, modPosition, 5.0f);
    }

    if (Input::IsKey(Key_Mouse1))
    {
        m_testObject1->Modify(VoxelEditMode::Additive, modPosition, 1.5f);
    }

    if (m_viewUpdateEnabled)
    {
        var cameraPosition = Camera::GetMainCamera()->GetPosition();
        m_testObject1->UpdateViewPoint(cameraPosition);
        m_testObject1->Update();
    }
}

void Universe::Simulate()
{
}

void Universe::Render()
{
    m_testObject1->Draw();
}
