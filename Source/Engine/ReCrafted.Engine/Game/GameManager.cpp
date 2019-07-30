// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "GameManager.h"
#include "Common/Signal.h"
#include "Content/ContentManager.h"
#include "Core/Transform.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Materials/Material.h"
#include "Scripting/Object.h"
#include "Scripting/ScriptingManager.h"
#include "Rendering/Models/ModelRenderingSystem.h"
#include "Profiler/Profiler.h"

ModelComponent* m_testModelComponent; 
Mesh* m_testMesh = nullptr;
Material* m_testMaterial = nullptr;
Transform m_testTransform = {};

GameManager::GameManager()
{
    m_game = Object::New("ReCrafted.Game.Core", "Game", ScriptingManager::GetGameAssembly());

    const auto gameInitialize = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnInitialize");
    ASSERT(gameInitialize.IsValid());

    m_gameShutdown = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnShutdown");
    ASSERT(m_gameShutdown.IsValid());

    m_gameUpdate = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnUpdate");
    ASSERT(m_gameUpdate.IsValid());

    m_gameFixedUpdate = m_game->GetClass().GetMethod("ReCrafted.Game.Core.Game::OnFixedUpdate");
    ASSERT(m_gameFixedUpdate.IsValid());

    gameInitialize.Invoke(m_game);

    m_testMesh = Mesh::CreateMesh();

    Vector3 cubeVertices[8] = {
        Vector3(-1.0f, 1.0f, -1.0f),    // 0  b---
        Vector3(-1.0f, 1.0f,  1.0f),    // 1  f---
        Vector3(1.0f, 1.0f,  1.0f),     // 2  ---f
        Vector3(1.0f, 1.0f, -1.0f),     // 3  ---b

        Vector3(-1.0f, -1.0f, -1.0f),   // 4  f---
        Vector3(-1.0f, -1.0f,  1.0f),   // 5  b---
        Vector3(1.0f, -1.0f,  1.0f),    // 6  ---f
        Vector3(1.0f, -1.0f, -1.0f),    // 7  ---b
    };

    Vector3 cubeNormals[8] = {
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),

        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
        Vector3(0.0f, 0.0f, 0.0f),
    };

    Vector2 cubeUVs[8] = {
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),

        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
        Vector2(0.0f, 0.0f),
    };

    uint32_t cubeIndices[36] = {
        0, 1, 2, 2, 3, 0, // top
        6, 5, 4, 4, 7, 6, // bottom

        7, 4, 0, 0, 3, 7, // front
        1, 5, 6, 6, 2, 1, // back

        1, 0, 5, 0, 4, 5, // left
        6, 7, 2, 7, 3, 2  // right
    };

    m_testMesh->SetVertices(Array<Vector3>(cubeVertices));
    m_testMesh->SetNormals(Array<Vector3>(cubeNormals));
    m_testMesh->SetUVs(Array<Vector2>(cubeUVs));
    m_testMesh->SetIndices(Array<uint>(cubeIndices));

    m_testMesh->ApplyChanges();

    m_testMaterial = ContentManager::LoadAsset<Material>("Materials/Default");

    // Acquire and setup model component for test render
    m_testModelComponent = ModelRenderingSystem::AcquireModelComponent();
    m_testModelComponent->Mesh = m_testMesh;
    m_testModelComponent->Material = m_testMaterial;
    m_testModelComponent->Transform = &m_testTransform;
    m_testModelComponent->Bounds = BoundingSphere(Vector3::Zero, 0.5f);
    m_testModelComponent->Active = true;
}

GameManager::~GameManager()
{
    // Destroy mesh
    Object::DestroyNow(m_testMesh);

    // Release test model component
    ModelRenderingSystem::ReleaseModelComponent(m_testModelComponent);
    m_testModelComponent = nullptr;

    ASSERT(m_game);
    ASSERT(m_gameShutdown.IsValid());

    m_gameShutdown.Invoke(m_game);

    // Destroy game
    Object::Destroy(m_game);
}

void GameManager::Update() const
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    DEBUG_ASSERT(m_game);
    DEBUG_ASSERT(m_gameUpdate.IsValid());

    m_gameUpdate.Invoke(m_game);
}

void GameManager::FixedUpdate() const
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    DEBUG_ASSERT(m_game);
    DEBUG_ASSERT(m_gameFixedUpdate.IsValid());

    m_gameFixedUpdate.Invoke(m_game);
}
