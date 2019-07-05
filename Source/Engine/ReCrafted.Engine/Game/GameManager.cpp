// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "GameManager.h"
#include "Common/Signal.h"
#include "Content/ContentManager.h"
#include "Rendering/Mesh.h"
#include "Rendering/RenderableBase.h"
#include "Rendering/Shader.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/Materials/Material.h"
#include "Scripting/Object.h"
#include "Scripting/ScriptingManager.h"

class RenderableTest : public RenderableBase
{
public:
    Vector3 pos = Vector3::Zero;
    BoundingBox bb = BoundingBox(Vector3::Zero, Vector3::One);
    Shader* sh = nullptr;
    Mesh* m_mesh = nullptr;

    RenderableTest()
    {
        sh = ContentManager::LoadAsset<Shader>("Shaders/StandardShader");

        m_mesh = Mesh::CreateMesh();

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

        uint32_t cubeIndices[36] = {
            0, 1, 2, 2, 3, 0, // top
            6, 5, 4, 4, 7, 6, // bottom

            7, 4, 0, 0, 3, 7, // front
            1, 5, 6, 6, 2, 1, // back

            1, 0, 5, 0, 4, 5, // left
            6, 7, 2, 7, 3, 2  // right
        };

        m_mesh->SetVertices(Array<Vector3>(cubeVertices));
        m_mesh->SetNormals(Array<Vector3>(cubeNormals));
        m_mesh->SetIndices(Array<uint>(cubeIndices));

        m_mesh->ApplyChanges();
    }

    ~RenderableTest()
    {
        Object::DestroyNow(m_mesh);
    }

    Mesh* GetMesh() const override
    {
        return m_mesh;
    }

    Shader* GetShader() const override
    {
        return sh;
    }

    Vector3& GetPosition() override
    {
        return pos;
    }

    BoundingBox& GetBounds() override
    {
        return bb;
    }
};

RenderableTest* test;

Signal signal;

void GameManager::tmpOnMaterialLoaded(Asset* materialAsset)
{

}

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

    test = new RenderableTest();
    RenderingManager::AddRenderable(test);

    ContentManager::LoadAsset<Material>("Materials/Default", Action<void, Asset*>::New<GameManager, &GameManager::tmpOnMaterialLoaded>(this));
}

GameManager::~GameManager()
{
    RenderingManager::RemoveRenderable(test);
    delete test;

    ASSERT(m_game);
    ASSERT(m_gameShutdown.IsValid());

    m_gameShutdown.Invoke(m_game);

    // Destroy game
    Object::Destroy(m_game);
}

void GameManager::Update() const
{
    DEBUG_ASSERT(m_game);
    DEBUG_ASSERT(m_gameUpdate.IsValid());

    m_gameUpdate.Invoke(m_game);
}

void GameManager::FixedUpdate() const
{
    DEBUG_ASSERT(m_game);
    DEBUG_ASSERT(m_gameFixedUpdate.IsValid());

    m_gameFixedUpdate.Invoke(m_game);
}
