// ReCrafted (c) 2016-2018 Always Too Late

#include "Universe.h"
#include "Common/EmptyActor.h"
#include "Common/Time.h"
#include "Common/Input/Input.h"
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
    settings.position = Vector3::Zero;
    settings.minSurfaceHeight = 900;
    settings.maxSurfaceHeight = settings.minSurfaceHeight + 120; // will round up to 1024 * 2
    settings.hillsHeight = 70;
    settings.rootOctreeDepth = 3; // 2 subdivisions (chunk size will be 512)

    m_testObject1 = SpaceObject::CreateSpaceObject(settings);

    // Generate primary data
    m_testObject1->GeneratePrimary();

    m_root = EmptyActor::Create();
    m_c1 = EmptyActor::Create();
    m_c2 = EmptyActor::Create();

    m_root->AddChild(m_c1);
    m_root->AddChild(m_c2);

    m_c1->SetLocalPosition(Vector3::Right * 3.0f);
    m_c2->SetLocalPosition(Vector3::Left * 3.0f);
}

void Universe::OnDispose()
{
    // Shutdown
    SafeDisposeNN(SpaceObjectManager::GetInstance()); // TODO: Fix issue with m_cooker being deleted before SpaceObject collision release!
    SafeDispose(m_testObject1);
}

float yRotation;
void Universe::Update()
{
    ASSERT(Camera::GetMainCamera());

    SpaceObjectManager::GetInstance()->Update();

    if (Input::IsKeyDown(Key_F7))
    {
        m_viewUpdateEnabled = !m_viewUpdateEnabled;
    }

    m_root->SetRotation(Quaternion::Rotation(Vector3::Up, yRotation));
    m_c1->SetRotation(Quaternion::Rotation(-m_c1->GetPosition().Normalized(), yRotation));


    DebugDraw::SetMatrix(m_root->GetTransform().GetTransformationMatrix());
    DebugDraw::DrawBox(Vector3::Zero, Vector3::One);

    DebugDraw::SetMatrix(m_c1->GetTransform().GetTransformationMatrix());
    DebugDraw::DrawBox(Vector3::Zero, Vector3::One * 0.25f);

    DebugDraw::SetMatrix(m_c2->GetTransform().GetTransformationMatrix());
    DebugDraw::DrawBox(Vector3::Zero, Vector3::One * 0.25f);

    
    if(Input::IsKeyDown(Key_F))
    {
        var projectile = RigidBodyActor::CreateDynamic();
        projectile->AttachCollision(Collision::CreateSphereCollision(0.1f));
        projectile->SetPosition(Camera::GetMainCamera()->GetPosition());
        projectile->SetVelocity(Camera::GetMainCamera()->GetForward() * 50.0f);
        m_projectiles.Add(projectile);
    }

    for (rvar box : m_projectiles)
    {
        DebugDraw::SetMatrix(Matrix::CreateRotation(box->GetRotation()) * Matrix::CreateTranslation(box->GetPosition()));
        DebugDraw::SetColor(Color(255, 110, 0, 255));
        DebugDraw::DrawBox(Vector3::Zero, Vector3::One * 0.2f);
    }
    DebugDraw::SetMatrix(Matrix::Identity);

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
        m_testObject1->Modify(m_selectedMaterial, VoxelEditMode::MaterialPaint, modPosition, 1.0f);
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
