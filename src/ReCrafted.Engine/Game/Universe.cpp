// ReCrafted (c) 2016-2018 Always Too Late

#include "Universe.h"
#include "Common/Input/Input.h"
#include "Common/Profiler/Profiler.h"
#include "Graphics/Camera.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/Graphics.h"
#include "Physics/PhysicsSystem.h"
#include "UI/UI.h"
#include "Voxels/SpaceObjectManager.h"
#include "Voxels/Storage/VoxelStorage.h"
#include "Voxels/SpaceObjectSettings.h"

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
    settings.hillsHeight = 70;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

    m_testObject1 = SpaceObject::CreateSpaceObject(settings);

    // Generate primary data
    m_testObject1->GeneratePrimary();

    // Create physics scene
    m_physicsScene = PhysicsSystem::Physics()->CreateScene();
}

void Universe::OnDispose()
{
    PhysicsSystem::Physics()->DestroyScene(m_physicsScene);

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

    if (Input::IsKeyDown(Key_Alpha1))
        m_selectedMaterial = 0u;
    if (Input::IsKeyDown(Key_Alpha2))
        m_selectedMaterial = 1u;
    if (Input::IsKeyDown(Key_Alpha3))
        m_selectedMaterial = 2u;
    if (Input::IsKeyDown(Key_Alpha4))
        m_selectedMaterial = 3u;
    if (Input::IsKeyDown(Key_Alpha5))
        m_selectedMaterial = 4u;
    if (Input::IsKeyDown(Key_Alpha6))
        m_selectedMaterial = 5u;

    if (Input::IsKey(Key_Mouse0))
    {
        m_testObject1->Modify(0u, VoxelEditMode::Subtractive, modPosition, 2.5f);
    }

    if (Input::IsKey(Key_Mouse1))
    {
        m_testObject1->Modify(m_selectedMaterial, VoxelEditMode::Additive, modPosition, 2.5f);
    }

    if (Input::IsKey(Key_Mouse2))
    {
        m_testObject1->Modify(m_selectedMaterial, VoxelEditMode::MaterialPaint, modPosition, 2.5f);
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

void Universe::RenderUI()
{
    switch(m_selectedMaterial)
    {
    case 0u:
        UI::DrawText(Profiler::GetInstance()->GetDebugFont(), TEXT_CONST("Selected material: Rock"), Vector2(10.0f, 10.0f));
        break;
    case 1u:
        UI::DrawText(Profiler::GetInstance()->GetDebugFont(), TEXT_CONST("Selected material: Grass"), Vector2(10.0f, 10.0f));
        break;
    case 2u:
        UI::DrawText(Profiler::GetInstance()->GetDebugFont(), TEXT_CONST("Selected material: Soil"), Vector2(10.0f, 10.0f));
        break;
    case 3u:
        UI::DrawText(Profiler::GetInstance()->GetDebugFont(), TEXT_CONST("Selected material: Clay"), Vector2(10.0f, 10.0f));
        break;
    case 4u:
        UI::DrawText(Profiler::GetInstance()->GetDebugFont(), TEXT_CONST("Selected material: Rock1"), Vector2(10.0f, 10.0f));
        break;
    case 5u:
        UI::DrawText(Profiler::GetInstance()->GetDebugFont(), TEXT_CONST("Selected material: Rock2"), Vector2(10.0f, 10.0f));
        break;
    }
}
