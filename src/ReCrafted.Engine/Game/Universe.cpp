// ReCrafted (c) 2016-2018 Always Too Late

#include "Universe.h"
#include "Common/Input/Input.h"
#include "Graphics/Camera.h"
#include "Voxels/SpaceObjectManager.h"
#include "Voxels/Storage/VoxelStorage.h"
#include "Voxels/SpaceObjectSettings.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/Graphics.h"

#include <iomanip>
#include <sstream>

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
    settings.fileName = "../assets/voxeldata/moon.chm";
    settings.saveName = "../saves/dev/voxelstorage/moon.vxh";
    settings.generationType = GenerationType::CubeHeightMap;
    settings.position = Vector3::Zero();
    settings.minSurfaceHeight = 900;
    settings.maxSurfaceHeight = settings.minSurfaceHeight + 120; // will round up to 1024 * 2
    settings.hillsHeight = 40;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

    m_testObject1 = SpaceObject::CreateSpaceObject(settings);

    // Generate primary data
    m_testObject1->GeneratePrimary();
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

    DebugDraw::SetColor(Color(0, 105, 0, 64));

    if (m_viewUpdateEnabled)
    {
        var cameraPosition = Camera::GetMainCamera()->GetPosition();
        m_testObject1->UpdateViewPoint(cameraPosition);
        m_testObject1->Update();
    }

    if (Input::IsKeyDown(Key_L))
    {
        var t = std::time(nullptr);
        var tm = *std::localtime(&t);

        std::stringstream fileName;
        fileName << std::put_time(&tm, "../screenshots/recrafted_%d-%m-%Y_%H-%M-%S.png");
        Graphics::Screenshot(Text(fileName.str().c_str()));
        Logger::Log("Screenshot saved as {0}", fileName.str());
    }
}

void Universe::Simulate()
{
}

void Universe::Render()
{
    m_testObject1->Draw();
}
