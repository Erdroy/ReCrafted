// ReCrafted (c) 2016-2018 Always Too Late

#include "Universe.h"
#include "Common/Profiler/Profiler.h"
#include "Common/Entities/MainWorld.h"
#include "Core/Logger.h"
#include "Graphics/Camera.h"
#include "Graphics/DebugDraw.h"
#include "Graphics/Graphics.h"
#include "Physics/PhysicsManager.h"
#include "UI/UI.h"
#include "Voxels/SpaceObjectManager.h"
#include "Voxels/Storage/VoxelStorage.h"
#include "Voxels/SpaceObjectSettings.h"

#include "imgui.h"
#include <iomanip>
#include <sstream>
#include "Input/InputManager.h"

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
    settings.position = Vector3::Zero;
    settings.minSurfaceHeight = 900;
    settings.maxSurfaceHeight = settings.minSurfaceHeight + 120; // will round up to 1024 * 2
    settings.hillsHeight = 70;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

    m_testObject1 = SpaceObject::CreateSpaceObject(settings);

    // Generate primary data
    m_testObject1->GeneratePrimary();

    m_ball = RigidBodyActor::CreateDynamic();
    m_ball->AttachCollision(Collision::CreateSphereCollision(0.5f));
}

void Universe::OnDispose()
{
    m_ball->Destroy();
    m_ball = nullptr;

    // Shutdown
    SafeDisposeNN(SpaceObjectManager::GetInstance()); // TODO: Fix issue with m_cooker being deleted before SpaceObject collision release!
    SafeDispose(m_testObject1);
}

void Universe::Update()
{
    ASSERT(Camera::GetMainCamera());

    SpaceObjectManager::GetInstance()->Update();

    if (InputManager::IsKeyDown(Key::F7))
    {
        m_viewUpdateEnabled = !m_viewUpdateEnabled;
    }

    cvar modPosition = Camera::GetMainCamera()->GetPosition() + Camera::GetMainCamera()->GetForward() * 5.0f;

    if (InputManager::IsKeyDown(Key::F))
    {
        m_ball->SetPosition(modPosition);
        m_ball->SetVelocity(Vector3::Zero);
        m_ball->AddForce(Camera::GetMainCamera()->GetForward() * 100.0f, ForceMode::VelocityChange);
    }

    if(InputManager::IsKey(Key::R))
    {
        m_ball->AddForce(Vector3::Normalize(m_ball->GetVelocity()) * 100.0f, ForceMode::Acceleration);
    }

    DebugDraw::SetMatrix(m_ball->GetTransform().GetTransformationMatrix());
    DebugDraw::SetColor(0xFFAA00FF);
    DebugDraw::DrawWireSphere(Vector3::Zero, 0.5f);
    DebugDraw::SetMatrix(Matrix::Identity);

    if (InputManager::IsKeyDown(Key::Alpha1))
        m_selectedMaterial = 0u;
    if (InputManager::IsKeyDown(Key::Alpha2))
        m_selectedMaterial = 1u;
    if (InputManager::IsKeyDown(Key::Alpha3))
        m_selectedMaterial = 2u;
    if (InputManager::IsKeyDown(Key::Alpha4))
        m_selectedMaterial = 3u;
    if (InputManager::IsKeyDown(Key::Alpha5))
        m_selectedMaterial = 4u;
    if (InputManager::IsKeyDown(Key::Alpha6))
        m_selectedMaterial = 5u;

    if (InputManager::IsButtonDown(Button::Left))
        DoVoxelModification(VoxelEditMode::Subtractive, 0u, 1.5f);

    if (InputManager::IsButtonDown(Button::Right))
        DoVoxelModification(VoxelEditMode::Additive, m_selectedMaterial, 1.5f);

    if (InputManager::IsButton(Button::Middle))
        DoVoxelModification(VoxelEditMode::MaterialPaint, m_selectedMaterial, 1.0f);

    DebugDraw::SetColor(Color(0, 105, 0, 64));

    if (m_viewUpdateEnabled)
    {
        var cameraPosition = Camera::GetMainCamera()->GetPosition();
        m_testObject1->UpdateViewPoint(cameraPosition);
        m_testObject1->Update();
    }

    if (InputManager::IsKeyDown(Key::L))
    {
        var t = std::time(nullptr);
        var tm = *std::localtime(&t);

        std::stringstream fileName;
        fileName << std::put_time(&tm, "../screenshots/recrafted_%d-%m-%Y_%H-%M-%S.png");
        Graphics::Screenshot(Text(fileName.str().c_str()));
        Logger::Log("Screenshot saved as {0}", fileName.str());
    }

    ImGui::Text("Selected material: %d", m_selectedMaterial);
}

void Universe::Simulate()
{
}

void Universe::DoVoxelModification(const VoxelEditMode::_enum mode, const VoxelMaterial_t material, const float size) const
{
    RayCastHit hit{};
    if (PhysicsManager::RayCast(Camera::GetMainCamera()->GetPosition(), Camera::GetMainCamera()->GetForward(), 5.0f, &hit, CollisionLayers::Default))
    {
        m_testObject1->Modify(material, mode, hit.point, size);
    }
}
